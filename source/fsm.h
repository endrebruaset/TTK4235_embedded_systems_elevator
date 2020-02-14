#include "hardware.h"

/**
 * @file
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
