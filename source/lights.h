/**
 * @file
 * @brief
 */


#include "hardware.h"
#include "queue.h"
// N: Var det dette vi ikke burde gjøre? Include hardware i lights?


#ifndef LIGHTS_H
#define LIGHTS_H


/**
 * @file
 * @brief Light-handling. 
 * /


/**
 * @brief Clears all order lights in case of state @c EMERGENCY_STOP.  
 */
void lights_clear_all_order_lights();

/**
 * @brief Clear all order lights on specific @p floor.
 * 
 * @param floor Desired floor to clear all order-lights. 
 */
void lights_clear_order_lights(int floor);


#endif
