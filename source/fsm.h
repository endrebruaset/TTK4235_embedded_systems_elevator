/**
 * @file
 * @brief Finite state machine that keeps track of which state the elevator is in, and performs the corresponding actions and transition. 
 */


#include "hardware.h"
#include "queue.h"
#include "lights.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>


#ifndef FSM_H
#define FSM_H
#define FSM_NOT_ON_FLOOR -1


/**
 * @brief All possible states the FSM can be in.
 */
typedef enum {
    MOVING,
    STAYING,
    IDLE,
    EMERGENCY_STOP
} State;


State m_current_state; ///< Current state of the FSM.
HardwareMovement m_moving_direction; ///< Current moving direction. Is not set to HARDWARE_MOVEMENT_STOP when the elevator stops.
HardwareMovement m_prev_moving_direction; ///< Previous moving direction.
int m_current_floor; ///< Current floor the elevator is on. Set to FSM_NOT_ON_FLOOR (-1) while not on floor.
int m_prev_floor; ///< Last defined floor the elevator was on.
int m_above_prev_floor; ///< Truthy value (1) if the elevator is above prev_floor, and a non-truthy value (0) if else.


/**
 * @brief Main program of the project, runs the finite state machine. Implemented in @c main.c.
 * 
 * @return 0 on success, truthy integer values if errors occur.
 */
int main();


/**
 * @brief Executes internal actions while in state @c MOVING.
 */
void fsm_in_state_moving();


/**
 * @brief Executes internal actions while in state @c STAYING.
 */
void fsm_in_state_staying();


/**
 * @brief Executes internal actions while in state @c IDLE.
 */
void fsm_in_state_idle();


/**
 * @brief Executes internal actions while in state @c EMERGENCY_STATE.
 */
void fsm_in_state_emergency_stop();


/**
 * @brief Executes entry actions in transition from @c m_current_state to @p next_state.
 * 
 * @param next_state State being transitioned to.
 */
void fsm_transition_to_state(State next_state);


/**
 * @brief Initializes the FSM, by moving downwards until a floor is reached. \
 * Ignores all orders and commands until it reaches a defined state. Sets \
 * all data members depending on floor, initializes queue and the timer, \
 * then enters state @c IDLE. 
 */
void fsm_initialize();


/**
 * @brief Polls all order buttons, and adds orders to queue. If an order is added, \
 * the corresponding order light is set.
 * 
 */ 
void fsm_read_orders_and_set_order_lights();


/**
 * @brief Removes all executed orders from the queue, and clears the corresponding order lights.
 * 
 * @param floor Floor at which the orders are executed.
 */
void fsm_remove_orders_and_clear_order_lights(int floor);


#endif
