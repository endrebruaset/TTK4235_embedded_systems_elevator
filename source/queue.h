#include "hardware.h"

#define INSIDE_QUEUE_SIZE      HARDWARE_NUMBER_OF_FLOORS
#define OUTSIDE_QUEUE_SIZE     2*(HARDWARE_NUMBER_OF_FLOORS - 1)

/**
 * @brief Data structure to be used in @c inside_queue, to keep track of orders from inside the elevator.
 */ 
typedef struct {
    int to_floor;       ///< Desired floor of order.
    int active;         ///< 1 if order exist, 0 if not.
} InsideOrder;


/**
 * @brief Data structure to be used in @c outside_queue, to keep track of orders from outside the elevator.
 */ 
typedef struct {
    int from_floor;             ///< Floor the order is made from.
    int active;                 ///< 1 if order exist, 0 if not.
    HardwareOrder direction;    ///< Direction of the order.
} OutsideOrder;


/**
 * @brief Queue containing all possible inside orders.
 */ 
InsideOrder inside_queue[INSIDE_QUEUE_SIZE] =
    {
        {0, 0},
        {1, 0},
        {2, 0},
        {3, 0}
    };


/**
 * @brief Queue containing all possible outside orders.
 */ 
OutsideOrder outside_queue[OUTSIDE_QUEUE_SIZE] =
    {
        {1, 0, HARDWARE_ORDER_DOWN},
        {2, 0, HARDWARE_ORDER_DOWN},
        {3, 0, HARDWARE_ORDER_DOWN},
        {0, 0, HARDWARE_ORDER_UP},
        {1, 0, HARDWARE_ORDER_UP},
        {2, 0, HARDWARE_ORDER_UP}
    };


/**
 * @brief Clears all orders from @c inside_queue and @c outside_queue, by setting the @c active element to an untruthy value (0) 
 * in all the orders.
 */ 
void clear_queue();


/**
 * @brief Adds order to @c inside_queue or @c outside_queue by setting the @c active element
 * to a truthy value (1) in the order corresponding to the input parameters.
 * 
 * @param floor Desired floor for inside orders, and floor the order is made from for outside orders.
 * 
 * @param order_type Type of order.
 */ 
void add_order(int floor, HardwareOrder order_type);


/**
 * @brief Removes all excecuted orders from or to @p floor.
 * 
 * @param floor The floor the elevator stops at to execute orders.
 * 
 */
void remove_executed_orders(int floor);

/**
 * @brief Polls all order buttons, and adds orders to their respective queues.
 * 
 * @return 1 if any order is added, 0 otherwise.
 */ 
int read_orders();


