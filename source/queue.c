#include "queue.h"
#include "queue_constants.h"


static InsideOrder m_inside_queue[QUEUE_INSIDE_QUEUE_SIZE]; ///< Queue containing all possible orders from inside the elevator.
static OutsideOrder m_outside_queue[QUEUE_OUTSIDE_QUEUE_SIZE]; ///< Queue containing all possible orders from outside the elevator.


void queue_initialize() {
    for (int f = 0; f < QUEUE_NUMBER_OF_FLOORS; f++) {
        InsideOrder order;
        order.to_floor = f;
        order.active = 0;

        m_inside_queue[f] = order;
    }

    for (int f = 0; f < (QUEUE_NUMBER_OF_FLOORS - 1); f++) {
        OutsideOrder order;
        order.from_floor = f;
        order.direction = QUEUE_ORDER_UP;
        order.active = 0;

        m_outside_queue[f] = order;
    }

    for (int f = 0; f < (QUEUE_NUMBER_OF_FLOORS - 1); f++) {
        OutsideOrder order;
        order.from_floor = f + 1;
        order.direction = QUEUE_ORDER_DOWN;
        order.active = 0;

        m_outside_queue[f + (QUEUE_OUTSIDE_QUEUE_SIZE / 2)] = order;
    }
}


void queue_clear() {
    for (int i = 0; i < QUEUE_INSIDE_QUEUE_SIZE; i++) {
        m_inside_queue[i].active = 0;
    }

    for (int i = 0; i < QUEUE_OUTSIDE_QUEUE_SIZE; i++) {
        m_outside_queue[i].active = 0;
    }
}


void queue_add_order(int floor, QueueOrder order_type) {
    if (order_type == QUEUE_ORDER_INSIDE) {
        for (int i = 0; i < QUEUE_INSIDE_QUEUE_SIZE; i++) {
            if (m_inside_queue[i].to_floor == floor) {
                m_inside_queue[i].active = 1;
            }
        }
    }

    else {
        for (int i = 0; i < QUEUE_OUTSIDE_QUEUE_SIZE; i++) {
            if (m_outside_queue[i].from_floor == floor && m_outside_queue[i].direction == order_type) {
                m_outside_queue[i].active = 1;
            }
        }
    }
}


void queue_remove_order(int floor, QueueOrder order_type) {
    if (order_type == QUEUE_ORDER_INSIDE) {
        for (int i = 0; i < QUEUE_INSIDE_QUEUE_SIZE; i++) {
            if (m_inside_queue[i].to_floor == floor) {
                m_inside_queue[i].active = 0;
            }
        }
    }

    else {
        for (int i = 0; i < QUEUE_OUTSIDE_QUEUE_SIZE; i++) {
            if (m_outside_queue[i].from_floor == floor && m_outside_queue[i].direction == order_type) {
                m_outside_queue[i].active = 0;
            }
        }
    }
}


int queue_check_order(int floor, QueueOrder order_type) {
    if (order_type == QUEUE_ORDER_INSIDE) {
        for (int i = 0; i < QUEUE_INSIDE_QUEUE_SIZE; i++) {
            if (m_inside_queue[i].to_floor == floor) {
                return m_inside_queue[i].active;
            }
        }
    }

    else {
        for (int i = 0; i < QUEUE_OUTSIDE_QUEUE_SIZE; i++) {
            if (m_outside_queue[i].from_floor == floor && m_outside_queue[i].direction == order_type) {
                return m_outside_queue[i].active;
            }
        }
    }

    return 0;
}


int queue_any_orders_on_floor(int floor) {
    int order_on_floor = 0;

    for (int i = 0; i < QUEUE_INSIDE_QUEUE_SIZE; i++) {
        if (m_inside_queue[i].to_floor == floor && m_inside_queue[i].active) {
            order_on_floor = 1;
        }
    }

    for (int i = 0; i < QUEUE_OUTSIDE_QUEUE_SIZE; i++) {
        if (m_outside_queue[i].from_floor == floor && m_outside_queue[i].active) {
            order_on_floor = 1;
        }
    }

    return order_on_floor;
}


int queue_any_orders_above_floor(int floor) {
    int order_above = 0;

    for (int i = 0; i < QUEUE_INSIDE_QUEUE_SIZE; i++) {
        if (m_inside_queue[i].to_floor > floor && m_inside_queue[i].active) {
            order_above = 1;
        }
    }

    for (int i = 0; i < QUEUE_OUTSIDE_QUEUE_SIZE; i++) {
        if (m_outside_queue[i].from_floor > floor && m_outside_queue[i].active) {
            order_above = 1;
        }
    }

    return order_above;
}


int queue_any_orders_below_floor(int floor) {
    int order_below = 0;

    for (int i = 0; i < QUEUE_INSIDE_QUEUE_SIZE; i++) {
        if (m_inside_queue[i].to_floor < floor && m_inside_queue[i].active) {
            order_below = 1;
        }
    }

    for (int i = 0; i < QUEUE_OUTSIDE_QUEUE_SIZE; i++) {
        if (m_outside_queue[i].from_floor < floor && m_outside_queue[i].active) {
            order_below = 1;
        }
    }

    return order_below;
}