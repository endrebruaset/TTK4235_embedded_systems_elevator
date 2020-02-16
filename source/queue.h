#include "hardware.h"


#ifndef QUEUE_H
#define QUEUE_H
#define INSIDE_QUEUE_SIZE      HARDWARE_NUMBER_OF_FLOORS
#define OUTSIDE_QUEUE_SIZE     2*(HARDWARE_NUMBER_OF_FLOORS - 1)


/**
 * @brief Data structure to be used in @c m_inside_queue, to keep track of orders from inside the elevator.
 */ 
typedef struct {
    int to_floor;       ///< Desired floor of order.
    int active;         ///< 1 if order exists, 0 if not.
} InsideOrder;


/**
 * @brief Data structure to be used in @c m_outside_queue, to keep track of orders from outside the elevator.
 */ 
typedef struct {
    int from_floor;             ///< Floor the order is made from.
    int active;                 ///< 1 if order exists, 0 if not.
    HardwareOrder direction;    ///< Direction of the order.
} OutsideOrder;


/**
 * @brief Queue containing all possible inside orders.
 */ 
static InsideOrder m_inside_queue[INSIDE_QUEUE_SIZE];


/**
 * @brief Queue containing all possible outside orders.
 */ 
static OutsideOrder m_outside_queue[OUTSIDE_QUEUE_SIZE];


/**
 * @brief Initializes @c m_inside_queue and @c m_outside_queue by inserting all possible combinations of \
 * order types and floors, with the @c active member set to 0.
 */
void queue_initialize();


/**
 * @brief Clears all orders from @c m_inside_queue and @c m_outside_queue, by setting the @c active element to an untruthy value (0) \
 * in all the orders.
 */ 
void queue_clear();


/**
 * @brief Adds order to @c m_inside_queue or @c m_outside_queue by setting the @c active element \
 * to a truthy value (1) in the order corresponding to the input parameters.
 * 
 * @param floor Desired floor for inside orders, and floor the order is made from for outside orders.
 * 
 * @param order_type Type of order.
 */ 
void queue_add_order(int floor, HardwareOrder order_type);


/**
 * @brief Removes all excecuted orders from or to @p floor.
 * 
 * @param floor The floor the elevator stops at to execute orders.
 * 
 */
void queue_remove_executed_orders(int floor);


/**
 * @brief Polls all order buttons, and adds orders to their respective queues.
 * 
 */ 
void queue_read_orders();


/**
 * @brief Checks if there are any active orders to or from a floor above @p floor, not including @p floor.
 * 
 * @return 1 if there are any active orders below, 0 if not.
 */
int queue_any_orders_above(int floor);


/**
 * @brief Checks if there are any active orders to or from a floor below @p floor, not including @p floor.
 * 
 * @return 1 if there are any active orders below, 0 if not.
 */
int any_orders_below(int floor);

#endif
