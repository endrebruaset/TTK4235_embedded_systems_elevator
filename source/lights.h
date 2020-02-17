#include "hardware.h"
#include "queue.h"
// N: Var det dette vi ikke burde gjøre? Include hardware i lights?

/**
 * @file
 * @brief Light-handling. 
 * /


/**
 * @brief Clears all order lights in case of state @c EMERGENCY_STOP.  
 */
void lights_clear_all_orders();

/**
 * @brief Clear all order lights on specific @p floor.
 * 
 * @param floor Desired floor to clear all order-lights. 
 */
void lights_clear_orders(int floor);


