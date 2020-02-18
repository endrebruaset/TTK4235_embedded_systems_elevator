/**
 * @file
 * @brief Finite state machine that keeps track of which state the elevator is in, and performs \
 * the corresponding actions and transition. 
 */


#include "hardware.h"
#include "queue.h"
#include "lights.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>


#ifndef FSM_H
#define FSM_H


/**
 * @brief Possible states is the FSM.
 */
typedef enum {
    MOVING,
    STAYING,
    IDLE,
    EMERGENCY_STOP
} State;


static State m_current_state;
static HardwareMovement m_moving_direction;
static HardwareMovement m_prev_moving_direction;
static int m_current_floor; // -1 while not on floor

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
 * @brief Executes exit and entry actions in transition from @c current_state to @p next_state.
 * 
 * @param next_state State being transitioned to.
 */
void fsm_transition_to_state(State next_state);


/**
 * @brief Initializes the FSM, by moving downwards until a floor is reached. \
 * Ignores all orders and commands until it reaches a defined state. Sets \
 * all data members depending on floor, then enters state @c IDLE. 
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
 * @param floor The floor at which the orders are executed.
 */
void fsm_remove_orders_and_clear_order_lights(int floor);


#endif
