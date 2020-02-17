#include "queue.h"


void queue_initialize() {
    for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++) {
        InsideOrder order;
        order.to_floor = f;
        order.active = 0;

        m_inside_queue[f] = order;
    }

    for (int f = 0; f < (HARDWARE_NUMBER_OF_FLOORS - 1); f++) {
        OutsideOrder order;
        order.from_floor = f;
        order.direction = HARDWARE_ORDER_UP;
        order.active = 0;

        m_outside_queue[f] = order;
    }

    for (int f = 1; f < HARDWARE_NUMBER_OF_FLOORS; f++) {
        OutsideOrder order;
        order.from_floor = f;
        order.direction = HARDWARE_ORDER_DOWN;
        order.active = 0;

        m_outside_queue[f + (OUTSIDE_QUEUE_SIZE / 2)] = order;
    }
}


void queue_clear() {
    for (int i = 0; i < INSIDE_QUEUE_SIZE; i++) {
        m_inside_queue[i].active = 0;
    }

    for (int i = 0; i < OUTSIDE_QUEUE_SIZE; i++) {
        m_outside_queue[i].active = 0;
    }
}


void queue_add_order(int floor, HardwareOrder order_type) {
    if (order_type == HARDWARE_ORDER_INSIDE) {
        m_inside_queue[floor].active = 1;
    }

    else {
        for (int i = 0; i < OUTSIDE_QUEUE_SIZE; i++) {
            if (m_outside_queue[i].from_floor == floor && m_outside_queue[i].direction == order_type) {
                m_outside_queue[i].active = 1;
            }
        }
    }
}


void queue_remove_executed_orders(int floor) {
    for (int i = 0; i < INSIDE_QUEUE_SIZE; i++) {
        if (m_inside_queue[i].to_floor == floor) {
            m_inside_queue[i].active = 0;
        }
    }

    for (int i = 0; i < OUTSIDE_QUEUE_SIZE; i++) {
        if (m_outside_queue[i].from_floor == floor) {
            m_outside_queue[i].active = 0;
        }
    }
}


int queue_any_orders_on_floor(int floor) {
    int order_on_floor = 0;

    for (int i = 0; i < INSIDE_QUEUE_SIZE; i++) {
        if (m_inside_queue[i].to_floor == floor && m_inside_queue[i].active) {
            order_on_floor = 1;
        }
    }

    for (int i = 0; i < OUTSIDE_QUEUE_SIZE; i++) {
        if (m_outside_queue[i].from_floor == floor && m_outside_queue[i].active) {
            order_on_floor = 1;
        }
    }

    return order_on_floor;
}


int queue_any_orders_above_floor(int floor) {
    int order_above = 0;

    for (int i = 0; i < INSIDE_QUEUE_SIZE; i++) {
        if (m_inside_queue[i].to_floor > floor && m_inside_queue[i].active) {
            order_above = 1;
        }
    }

    for (int i = 0; i < OUTSIDE_QUEUE_SIZE; i++) {
        if (m_outside_queue[i].from_floor > floor && m_outside_queue[i].active) {
            order_above = 1;
        }
    }

    return order_above;
}


int queue_any_orders_below_floor(int floor) {
    int order_below = 0;

   for (int i = 0; i < INSIDE_QUEUE_SIZE; i++) {
        if (m_inside_queue[i].to_floor < floor && m_inside_queue[i].active) {
            order_below = 1;
        }
    }

    for (int i = 0; i < OUTSIDE_QUEUE_SIZE; i++) {
        if (m_outside_queue[i].from_floor < floor && m_inside_queue[i].active) {
            order_below = 1;
        }
    }

    return order_below;
}
