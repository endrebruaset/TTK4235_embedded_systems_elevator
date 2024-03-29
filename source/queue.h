/**
 * @file
 * @brief Module for queueing orders for the elevator.
 */


#ifndef QUEUE_H
#define QUEUE_H


/**
 * @brief Order type to be used in @c queue_add_order, @c queue_remove_order, and @c queue_check_order.
 */
typedef enum {
    QUEUE_ORDER_UP,
    QUEUE_ORDER_INSIDE,
    QUEUE_ORDER_DOWN
} QueueOrder;


/**
 * @brief Data structure to be used to represent orders from inside the elevator.
 */ 
typedef struct {
    int to_floor;       ///< Desired floor of order.
    int active;         ///< 1 if order exists, 0 if not.
} InsideOrder;


/**
 * @brief Data structure to be used to represent orders from outside the elevator.
 */ 
typedef struct {
    int from_floor;             ///< Floor the order is made from.
    int active;                 ///< 1 if order exists, 0 if not.
    QueueOrder direction;       ///< Direction of the order.
} OutsideOrder;


/**
 * @brief Initializes the queue by inserting all possible combinations of
 * order types and floors, with the @c active member set to 0.
 */
void queue_initialize();


/**
 * @brief Clears all orders from the queue by setting the @c active element to
 * an untruthy value (0) in all the orders.
 */ 
void queue_clear();


/**
 * @brief Adds order to the queue by setting the @c active element
 * to a truthy value (1) in the order corresponding to the input parameters.
 * 
 * @param floor Desired floor for inside orders, and floor the order is made from for outside orders.
 * @param order_type Type of order.
 */ 
void queue_add_order(int floor, QueueOrder order_type);


/**
 * @brief Removes order from the queue by setting the @c active element
 * to a non-truthy value (0) in the order corresponding to the input parameters.
 * 
 * @param floor Desired floor for inside orders, and floor the order is made from for outside orders.
 * @param order_type Type of order.
 */ 
void queue_remove_order(int floor, QueueOrder order_type);


/**
 * @brief Checks if there is an active order on @p floor of type @p order_type.
 * 
 * @param floor Floor to check for order.
 * @param order_type Type of order. 
 * 
 * @return 1 if the order of type @p type to or from floor @p floor is active, 0 if not.
 * 
 * @warning Returns 0 if the combination of @p floor and @p order_type is invalid.
 */

int queue_check_order(int floor, QueueOrder order_type);


/**
 * @brief Checks if there are any active orders to or from the floor @p floor.
 * 
 * @param floor Floor to check for order.
 * 
 * @return 1 if there are any active orders on @p floor, 0 if not.
 */
int queue_any_orders_on_floor(int floor);


/**
 * @brief Checks if there are any active orders to or from a floor above @p floor, not including @p floor.
 * 
 *  @param floor Floor to check for orders.
 * 
 * @return 1 if there are any active orders above @p floor, 0 if not.
 */
int queue_any_orders_above_floor(int floor);


/**
 * @brief Checks if there are any active orders to or from a floor below @p floor, not including @p floor.
 * 
 *  @param floor Floor to check for orders.
 * 
 * @return 1 if there are any active orders below @p floor, 0 if not.
 */
int queue_any_orders_below_floor(int floor);


#endif
