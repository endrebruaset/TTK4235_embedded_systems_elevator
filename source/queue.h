#include "hardware.h"

#define INSIDE_QUEUE_SIZE      HARDWARE_NUMBER_OF_FLOORS
#define OUTSIDE_QUEUE_SIZE     2*(HARDWARE_NUMBER_OF_FLOORS - 1)

typedef struct {
    int to_floor;
    int active;
} InsideOrder;


typedef struct {
    int from_floor;
    int active;
    HardwareOrder direction;
} OutsideOrder;


InsideOrder inside_queue[INSIDE_QUEUE_SIZE] =
    {
        {0, 0},
        {1, 0},
        {2, 0},
        {3, 0}
    };


OutsideOrder outside_queue[OUTSIDE_QUEUE_SIZE] =
    {
        {1, 0, HARDWARE_ORDER_DOWN},
        {2, 0, HARDWARE_ORDER_DOWN},
        {3, 0, HARDWARE_ORDER_DOWN},
        {0, 0, HARDWARE_ORDER_UP},
        {1, 0, HARDWARE_ORDER_UP},
        {2, 0, HARDWARE_ORDER_UP}
    };


void clear_queue();

void add_order(int floor, HardwareOrder);

void remove_executed_orders(int floor);

/**
 * @brief Polls all order buttons, and adds orders to their respective queues.
 * 
 * @return 1 if any order is added, 0 otherwise.
 */ 
int read_orders();


