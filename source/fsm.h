#include "hardware.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * @file
 * @brief Finite state machine that keeps track of which state the elevator is in and the transition between two states. 
 */

typedef enum {
    MOVING,
    STAYING,
    IDLE,
    EMERGENCY_STOP
} State;

State current_state;
HardwareMovement moving_direction;
HardwareMovement prev_moving_direction;
int current_floor;

/**
 * 
 */
void in_state_moving();

/**
 * 
 */
void in_state_staying();

/**
 * 
 */
void in_state_idle();

/**
 * 
 */
void in_state_emergency_stop();

/**
 * 
 */
void transition_to(State next_state);

/**
 * 
 */
void get_to_defined_state();
