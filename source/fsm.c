#include "fsm.h"

 
void fsm_in_state_moving() {
    if (hardware_read_stop_signal()) {
            fsm_transition_to_state(EMERGENCY_STOP);
        }

    fsm_read_orders_and_set_order_lights();

    for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; ++f) {
        if (hardware_read_floor_sensor(f)) {
            m_current_floor = f;
            hardware_command_floor_indicator_on(m_current_floor);
            
            if (queue_any_orders_on_floor(f)) {
                if (queue_check_order(f, m_moving_direction) || queue_check_order(f, HARDWARE_ORDER_INSIDE)) {
                    fsm_transition_to_state(STAYING);
                }
            }
        }
    }
}


void fsm_in_state_staying() {
    hardware_command_door_open(1);
    // start timer
    timer_set(DEFAULT_TIME_DOOR_OPEN);

    while(!(timer_is_elapsed())) {      
        if (hardware_read_stop_signal()) {
            fsm_transition_to_state(EMERGENCY_STOP);
        }

        fsm_read_orders_and_set_order_lights();

        if (hardware_read_obstruction_signal()) {
            // restart timer
            timer_set(DEFAULT_TIME_DOOR_OPEN);
        }
    }

    // timer elapses
    hardware_command_door_open(0);

    fsm_remove_orders_and_clear_order_lights(m_current_floor);

    // if empty queue
    if (!(queue_any_orders_above_floor(m_current_floor) || 
          queue_any_orders_below_floor(m_current_floor) ||
          queue_any_orders_on_floor(m_current_floor))) {
        fsm_transition_to_state(IDLE);
    }

    else if (queue_any_orders_on_floor(m_current_floor)) {
        fsm_transition_to_state(STAYING);
    }

    else {
        switch (m_prev_moving_direction)
        {
            case HARDWARE_MOVEMENT_DOWN: 
            {
                if (queue_any_orders_below_floor(m_current_floor)) {
                    m_moving_direction = HARDWARE_MOVEMENT_DOWN;
                }
                else {
                    m_moving_direction = HARDWARE_MOVEMENT_UP;
                }

                break;
            }

            case HARDWARE_MOVEMENT_UP: 
            {
                if (queue_any_orders_above_floor(m_current_floor)) {
                    m_moving_direction = HARDWARE_MOVEMENT_UP;
                }
                else {
                    m_moving_direction = HARDWARE_MOVEMENT_DOWN;
                }

                break;
            }
            default:
            {
                fprintf(stderr, "Impossible combination of state and m_prev_movement_direction");
                exit(1);
            }
        }
        fsm_transition_to_state(MOVING);
    }
}


void fsm_in_state_idle() {
    // coming from state EMERGENCY_STOP
    while (!timer_is_elapsed()) {
        if (hardware_read_stop_signal()) {
            fsm_transition_to_state(EMERGENCY_STOP);
        }

        if (hardware_read_obstruction_signal()) {
            while (hardware_read_obstruction_signal()){
                // keep door open
                fsm_read_orders_and_set_order_lights();
                timer_set(DEFAULT_TIME_DOOR_OPEN);
            }
        }

        fsm_read_orders_and_set_order_lights();
    }
    
    hardware_command_door_open(0);

    // coming from state STAYING
    fsm_read_orders_and_set_order_lights();

    if (queue_any_orders_on_floor(m_current_floor)) {
        fsm_transition_to_state(STAYING);
    }

    if (queue_any_orders_below_floor(m_current_floor)) {
        m_moving_direction = HARDWARE_MOVEMENT_DOWN;
        fsm_transition_to_state(MOVING);
    }

    else if (queue_any_orders_above_floor(m_current_floor)) {
        m_moving_direction = HARDWARE_MOVEMENT_UP;
        fsm_transition_to_state(MOVING);
    }
}


void fsm_in_state_emergency_stop() {
    if (m_current_floor != -1) {
        hardware_command_door_open(1);
    }

    while (hardware_read_stop_signal()) {
      // do nothing  
    }
    hardware_command_stop_light(0); 

    if (m_current_floor != -1) {
        timer_set(DEFAULT_TIME_DOOR_OPEN);
    }

    fsm_transition_to_state(IDLE);
}


void fsm_transition_to_state(State next_state) {
    switch(next_state) {
        case MOVING:
        {
            // not on floor
            m_current_floor = -1;

            hardware_command_movement(m_moving_direction);

            m_current_state = MOVING;

            break;
        }
        case STAYING:
        {
            m_prev_moving_direction = m_moving_direction;
            m_moving_direction = HARDWARE_MOVEMENT_STOP;
            hardware_command_movement(m_moving_direction); 

            m_current_state = STAYING;
            break;
        }
        case IDLE:
        {
            m_current_state = IDLE;
            break;
        }
        case (EMERGENCY_STOP):
        {
            hardware_command_stop_light(1); 

            m_moving_direction = HARDWARE_MOVEMENT_STOP;
            hardware_command_movement(m_moving_direction);

            queue_clear();
            lights_clear_all_order_lights();
            // clear all order lights

            m_current_state = EMERGENCY_STOP;
            break;
        }
        default:
        {
            fprintf(stderr, "Unable to transition to next state.\n");
            exit(1);
        }
    }
}


void fsm_initialize() { 
    while (1) {
        for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++) {
            if (hardware_read_floor_sensor(f)) {
                m_current_floor = f;

                m_moving_direction = HARDWARE_MOVEMENT_STOP;
                m_prev_moving_direction = HARDWARE_MOVEMENT_DOWN;
                hardware_command_movement(m_moving_direction);

                m_current_state = IDLE;
                return;
            }
        }

        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
}


void fsm_read_orders_and_set_order_lights() {    
    HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++) {
        for (int i = 0; i < 3; i++) {
            HardwareOrder type = order_types[i];

            if (hardware_read_order(f, type)) {
                queue_add_order(f, type);
                hardware_command_order_light(f, type, 1); 
            }
        }
    }
}


void fsm_remove_orders_and_clear_order_lights(int floor) {
    HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for (int i = 0; i < 3; i++) {
        HardwareOrder type = order_types[i];
        queue_remove_order(floor, type);
        hardware_command_order_light(floor, type, 0);
    }
}
