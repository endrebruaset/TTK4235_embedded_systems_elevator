/**
 * @file
 * @brief Implementation of the finite state machine.
 */


#include "fsm.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>


static State m_current_state; ///< Current state of the FSM.

static HardwareMovement m_moving_direction; ///< Current moving direction. Is not set to HARDWARE_MOVEMENT_STOP when the elevator stops.
static HardwareMovement m_prev_moving_direction; ///< Previous moving direction.

static int m_current_floor; ///< Current floor the elevator is on. Set to FSM_NOT_ON_FLOOR (-1) while not on floor.
static int m_prev_floor; ///< Last defined floor the elevator was on.
static int m_above_prev_floor; ///< Truthy value (1) if the elevator is above the previous floor, and a non-truthy value (0) if not.


static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}


int main() {
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    signal(SIGINT, sigint_handler);


    fsm_initialize();
    
    
    while(1) {
        switch (m_current_state)
        {
            case MOVING:
            {
                fsm_in_state_moving();
                break;
            }

            case STAYING:
            {
                fsm_in_state_staying();
                break;
            }

            case IDLE:
            {
                fsm_in_state_idle();
                break;
            }

            case EMERGENCY_STOP:
            {
                fsm_in_state_emergency_stop();
                break;
            }

            default:
                fprintf(stderr, "Undefined state");
                exit(2);
                break;
        }
    }

    return 0;
}

 
void fsm_in_state_moving() {
    if (hardware_read_stop_signal()) {
            fsm_transition_to_state(EMERGENCY_STOP);
            return;
        }

    fsm_read_orders_and_set_order_lights();

    for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; ++f) {
        if (hardware_read_floor_sensor(f)) {
            m_current_floor = f;
            hardware_command_floor_indicator_on(m_current_floor);

            m_prev_floor = m_current_floor;
            if (m_moving_direction == HARDWARE_MOVEMENT_UP) {m_above_prev_floor = 1;}
            else if (m_moving_direction == HARDWARE_MOVEMENT_DOWN) {m_above_prev_floor = 0;}

            break;
        }
        else {
            m_current_floor = FSM_NOT_ON_FLOOR;
        }
    }        
    
    if (m_current_floor != FSM_NOT_ON_FLOOR) {
        if (queue_any_orders_on_floor(m_current_floor)) {
            if (!queue_any_orders_above_floor(m_current_floor) && m_moving_direction == HARDWARE_MOVEMENT_UP) {
                fsm_transition_to_state(STAYING);
            }

            if (!queue_any_orders_below_floor(m_current_floor) && m_moving_direction == HARDWARE_MOVEMENT_DOWN) {
                fsm_transition_to_state(STAYING);
            }

            // m_moving_direction has an enum value corresponding to queue_order_up or queue_order_down
            if (queue_check_order(m_current_floor, (QueueOrder) m_moving_direction) || queue_check_order(m_current_floor, HARDWARE_ORDER_INSIDE)) {
                fsm_transition_to_state(STAYING);
            }
        }
    }     
}


void fsm_in_state_staying() {
    timer_set(FSM_DEFAULT_TIME_DOOR_OPEN);

    while(!(timer_is_elapsed())) {      
        if (hardware_read_stop_signal()) {
            fsm_transition_to_state(EMERGENCY_STOP);
            return;
        }

        fsm_read_orders_and_set_order_lights();

        if (hardware_read_obstruction_signal()) {
            timer_set(FSM_DEFAULT_TIME_DOOR_OPEN);
        }
    }

    fsm_remove_orders_and_clear_order_lights(m_current_floor);

    hardware_command_door_open(0);


    // if queue is empty
    if (!(queue_any_orders_above_floor(m_current_floor) || 
          queue_any_orders_below_floor(m_current_floor) ||
          queue_any_orders_on_floor(m_current_floor))) {
        fsm_transition_to_state(IDLE);
    }

    else {
        if (m_prev_moving_direction == HARDWARE_MOVEMENT_UP) {
            if (queue_any_orders_above_floor(m_current_floor)) {
                m_moving_direction = HARDWARE_MOVEMENT_UP;
            }
            else {
                m_moving_direction = HARDWARE_MOVEMENT_DOWN;
            }
        }
        
        else {
            if (queue_any_orders_below_floor(m_current_floor)) {
                m_moving_direction = HARDWARE_MOVEMENT_DOWN;
            }
            else {
                m_moving_direction = HARDWARE_MOVEMENT_UP;
            }
        }

        fsm_transition_to_state(MOVING);
    }
}


void fsm_in_state_idle() {
    // if door is open, becuase previous state was emergency_stop and the elevator is on a floor
    if (!timer_is_elapsed()) {
        while (!timer_is_elapsed()) {
            if (hardware_read_stop_signal()) {
                fsm_transition_to_state(EMERGENCY_STOP);
                return;
            }

            if (hardware_read_obstruction_signal()) {
                timer_set(FSM_DEFAULT_TIME_DOOR_OPEN);
            }

            fsm_read_orders_and_set_order_lights();
        }
    
        hardware_command_door_open(0);
    }

    if (hardware_read_stop_signal()) {
            fsm_transition_to_state(EMERGENCY_STOP);
            return;
    }

    fsm_read_orders_and_set_order_lights();

    if (m_current_floor != FSM_NOT_ON_FLOOR) {
        if (queue_any_orders_on_floor(m_current_floor)) {
            fsm_transition_to_state(STAYING);
        }

        else if (queue_any_orders_below_floor(m_current_floor)) {
            m_moving_direction = HARDWARE_MOVEMENT_DOWN;
            fsm_transition_to_state(MOVING);
        }

        else if (queue_any_orders_above_floor(m_current_floor)) {
            m_moving_direction = HARDWARE_MOVEMENT_UP;
            fsm_transition_to_state(MOVING);
        }
    }

    else {
        if (queue_any_orders_on_floor(m_prev_floor)) {
            if (m_above_prev_floor) { 
                m_moving_direction = HARDWARE_MOVEMENT_DOWN;
            }
            else {
                m_moving_direction = HARDWARE_MOVEMENT_UP;
            }
            fsm_transition_to_state(MOVING);
        }

        else if (queue_any_orders_below_floor(m_prev_floor)) {
            m_moving_direction = HARDWARE_MOVEMENT_DOWN;
            fsm_transition_to_state(MOVING);
        }

        else if (queue_any_orders_above_floor(m_prev_floor)) {
            m_moving_direction = HARDWARE_MOVEMENT_UP;
            fsm_transition_to_state(MOVING);
        }
    }
}


void fsm_in_state_emergency_stop() {
    if (m_current_floor != FSM_NOT_ON_FLOOR) {
        hardware_command_door_open(1);
    }

    while (hardware_read_stop_signal()) {
      // do nothing  
    }
    hardware_command_stop_light(0); 

    if (m_current_floor != FSM_NOT_ON_FLOOR) {
        timer_set(FSM_DEFAULT_TIME_DOOR_OPEN);
    }

    fsm_transition_to_state(IDLE);
}


void fsm_transition_to_state(State next_state) {
    switch(next_state) {
        case MOVING:
        {
            hardware_command_movement(m_moving_direction);

            m_current_state = MOVING;

            break;
        }
        case STAYING:
        {
            m_prev_moving_direction = m_moving_direction;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);

            hardware_command_door_open(1);

            m_current_state = STAYING;
            break;
        }
        case IDLE:
        {
            m_current_state = IDLE;
            break;
        }
        case EMERGENCY_STOP:
        {
            hardware_command_stop_light(1); 

            m_prev_moving_direction = m_moving_direction;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);

            queue_clear();
            fsm_clear_all_order_lights();

            m_current_state = EMERGENCY_STOP;
            break;
        }
        default:
        {
            fprintf(stderr, "Unable to transition to next state.\n");
            exit(3);
            break;
        }
    }
}


void fsm_initialize() { 
    while (1) {
        for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++) {
            if (hardware_read_floor_sensor(f)) {
                m_current_floor = f;
                hardware_command_floor_indicator_on(f);

                m_prev_moving_direction = HARDWARE_MOVEMENT_DOWN;
                hardware_command_movement(HARDWARE_MOVEMENT_STOP);

                queue_initialize();
                timer_set(0);
                fsm_clear_all_order_lights();

                m_current_state = IDLE;
                return;
            }
        }

        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
}


void fsm_read_orders_and_set_order_lights() {    
    HardwareOrder order_types[HARDWARE_NUMBER_OF_ORDER_TYPES] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++) {
        for (int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++) {
            HardwareOrder type = order_types[i];

            if (hardware_read_order(f, type)) {
                queue_add_order(f, (QueueOrder) type);
                hardware_command_order_light(f, type, 1); 
            }
        }
    }
}


void fsm_remove_orders_and_clear_order_lights(int floor) {
    HardwareOrder order_types[HARDWARE_NUMBER_OF_ORDER_TYPES] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for (int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++) {
        HardwareOrder type = order_types[i];
        queue_remove_order(floor, (QueueOrder) type);
        hardware_command_order_light(floor, type, 0);
    }
}


void fsm_clear_all_order_lights() {
    HardwareOrder order_types[HARDWARE_NUMBER_OF_ORDER_TYPES] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++){
            HardwareOrder type = order_types[i];
            hardware_command_order_light(f, type, 0);
        }
    }
}