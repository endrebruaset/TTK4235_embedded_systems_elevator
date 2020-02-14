#include "hardware.h"
#include "queue.h"

/**
 * @file
 */

/**
 * @brief Possible states is the FSM.
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
int current_floor; // -1 while not on floor

/**
 * @brief Executes internal actions while in state @c MOVING.
 */
void in_state_moving();

/**
 * @brief Executes internal actions while in state @c STAYING.
 */
void in_state_staying();

/**
 * @brief Executes internal actions while in state @c IDLE.
 */
void in_state_idle();

/**
 * @brief Executes internal actions while in state @c EMERGENCY_STATE.
 */
void in_state_emergency_stop();

/**
 * @brief Executes exit and entry actions in transition from @c current_state to @p next_state.
 * 
 * @param next_state State being transitioned to.
 */
void transition_to(State next_state);

/**
 * @brief Initializes the FSM, by moving downwards until a floor is reached.
 * Ignores all orders and commands until it reaches a defined state. Then enters state @c IDLE. 
 */
void get_to_defined_state();
