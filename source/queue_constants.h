/**
 * @file
 * @brief Constants to be used in the queue module, 
 * defined through the constant HARDWARE_NUMBER_OF_FLOORS
 * defined in @c hardware.h.
 */


#include "hardware.h"


#ifndef QUEUE_CONSTANTS_H
#define QUEUE_CONSTANTS_H


#define QUEUE_NUMBER_OF_FLOORS       HARDWARE_NUMBER_OF_FLOORS
#define QUEUE_INSIDE_QUEUE_SIZE      QUEUE_NUMBER_OF_FLOORS
#define QUEUE_OUTSIDE_QUEUE_SIZE     2*(QUEUE_NUMBER_OF_FLOORS - 1)


#endif