#include "fsm.h"

 
void in_state_moving() {

}


// If obstruction switch is activated: keep door open
void in_state_staying() {

}


void in_state_idle() {
    //read_orders();
    // implementere neworder i kømodul
    if (read_orders()) {
        transition_to(MOVING);
    }
    if (hardware_read_stop_signal()) {
        transition_to(EMERGENCY_STOP);
    } 

}

// If obstruction switch is activated: keep door open
void in_state_emergency_stop() {
    if (current_floor != -1) {
        hardware_command_door_open(1);
    }

    while (hardware_read_stop_signal()) {
      // do nothing  
    }
    hardware_command_stop_light(0); 

    // Start timer 3 seconds
    if (hardware_read_stop_signal()) {
        // stop timer
        // state remains emergency_stop
    }
    else {
        // continue timer
        hardware_command_door_open(0);
        transition_to(IDLE);
    }
}


void transition_to(State next_state) {
    switch(next_state) {
        case (MOVING):
            // Code for exit action current_state + entry action next_state MOVING
            current_state = MOVING;
            break;
        case (STAYING):
            // Code for exit action current_state + entry action next_state STAYING
            current_state = STAYING;
            break;
        case (IDLE):
            // Code for exit action current_state + entry action next_state IDLE
            current_state = IDLE;
            break;
        case (EMERGENCY_STOP):
            // Code for exit action current_state + entry action next_state EMERGENCY_STOP
            hardware_command_stop_light(1); 
            moving_direction = HARDWARE_MOVEMENT_STOP;
            hardware_command_movement(moving_direction);
            // clear queue
            current_state = EMERGENCY_STOP;


            break;
        default:
            fprintf(stderr, "Unable to transition to next state.\n");
            exit(1);
            
    }
}


void get_to_defined_state() { 

}