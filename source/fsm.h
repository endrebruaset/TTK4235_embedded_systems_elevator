#include "hardware.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>


#ifndef FSM_H
#define FSM_H


/**
 * @file
 * @brief Finite state machine that keeps track of which state the elevator is in and the transition between two states. 
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


State g_current_state;
static HardwareMovement m_moving_direction;
static HardwareMovement m_prev_moving_direction;
static int m_current_floor; // -1 while not on floor


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

#endif
