#include "fsm.h"

 
void fsm_in_state_moving() {
    // update current floor
    if (hardware_read_stop_signal()) {
            transition_to_state(EMERGENCY_STOP);
        }
        
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i) {
        if (hardware_read_floor_sensor(i)) {
             m_current_floor = i;

            if (queue_any_orders_on_floor(i)) {
                transition_to_state(STAYING);
            }
        }
    }
}



// If obstruction switch is activated: keep door open
void fsm_in_state_staying() {
    hardware_command_door_open(1);
    // start timer

    // while timer has not elapsed
        fsm_read_orders();
        // sett ordrelys
        if (hardware_read_stop_signal()) {
            transition_to_state(EMERGENCY_STOP);
        }
        if (hardware_read_obstruction_signal()) {
            // restart timer
        }

    // timer elapses
    hardware_command_door_open(0);

    queue_remove_executed_orders(m_current_floor);
    // clear order lights from current_floor

    // if empty queue
    if (!(queue_any_orders_above_floor(m_current_floor) || 
          queue_any_orders_below_floor(m_current_floor) ||
          queue_any_orders_on_floor(m_current_floor))) {
        transition_to_state(IDLE);
    }

    else if (queue_any_orders_on_floor(m_current_floor)) {
        transition_to_state(STAYING);
    }

    else {
        switch (m_prev_moving_direction)
        {
            // last moving down
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

            // last moving up
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
        transition_to_state(MOVING);
    }
}


void fsm_in_state_idle() {
    if (hardware_read_stop_signal()) {
        transition_to_state(EMERGENCY_STOP);
    } 

    fsm_read_orders();

    // if orders on current floor transition to staying

    if (queue_any_orders_below_floor(m_current_floor)) {
        m_moving_direction = HARDWARE_MOVEMENT_DOWN;
        transition_to_state(MOVING);
    }

    else if (queue_any_orders_above_floor(m_current_floor)) {
        m_moving_direction = HARDWARE_MOVEMENT_UP;
        transition_to_state(MOVING);
    }
}


// If obstruction switch is activated: keep door open
void fsm_in_state_emergency_stop() {
    if (m_current_floor != -1) {
        hardware_command_door_open(1);
    }

    while (hardware_read_stop_signal()) {
      // do nothing  
    }
    hardware_command_stop_light(0); 

    // diskuter om handlingene under bør gjøres i idle, dvs. at man kun er i emergency_stop mens stoppknappen er trykket inne.
    // husk at timer kan gå på tvers av states.

    // start timer 3 seconds
    if (hardware_read_stop_signal()) {
        // stop timer
        // state remains emergency_stop
    }
    else {
        // continue timer
        if (hardware_read_obstruction_signal()) {
            while (hardware_read_obstruction_signal()){
                // keep door open
                fsm_read_orders();
            }
            // restart timer
        }
        hardware_command_door_open(0);
        transition_to_state(IDLE);
    }
}


void fsm_transition_to_state(State next_state) {
    switch(next_state) {
        case MOVING:
        {
            // Code for exit action current_state + entry action next_state MOVING

            // not on floor
            m_current_floor = -1;

            hardware_command_movement(m_moving_direction);

            m_current_state = MOVING;

            break;
        }
        case STAYING:
        {
            // Code for exit action current_state + entry action next_state STAYING
            m_prev_moving_direction = m_moving_direction;
            m_moving_direction = HARDWARE_MOVEMENT_STOP;
            hardware_command_movement(m_moving_direction);

            hardware_command_floor_indicator_on(m_current_floor);

            m_current_state = STAYING;
            break;
        }
        case IDLE:
        {
            // Code for exit action current_state + entry action next_state IDLE
            m_current_state = IDLE;
            break;
        }
        case (EMERGENCY_STOP):
        {
            // Code for exit action current_state + entry action next_state EMERGENCY_STOP
            hardware_command_stop_light(1); 

            m_moving_direction = HARDWARE_MOVEMENT_STOP;
            hardware_command_movement(m_moving_direction);

            queue_clear();
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


void fsm_read_orders() {    
    HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < 3; i++){
            HardwareOrder type = order_types[i];

            if (hardware_read_order(f, type)) {
                queue_add_order(f, type);
            }
        }
    }
}