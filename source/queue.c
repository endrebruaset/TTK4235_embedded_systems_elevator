#include "queue.h"


void clear_queue() {
    for (int i = 0; i < INSIDE_QUEUE_SIZE; i++) {
        inside_queue[i].active = 0;
    }

    for (int i = 0; i < OUTSIDE_QUEUE_SIZE; i++) {
        outside_queue[i].active = 0;
    }
}

void add_order(int floor, HardwareOrder order_type) {
    if (order_type == HARDWARE_ORDER_INSIDE) {
        inside_queue[floor].active = 1;
    }

    else {
        for (int i = 0; i < OUTSIDE_QUEUE_SIZE; i++) {
            if (outside_queue[i].from_floor == floor && outside_queue[i].direction == order_type) {
                outside_queue[i].active = 1;
            }
        }
    }
}

void remove_executed_orders(int floor) {

}


int read_orders() {
    int any_order = 0;
    
    HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < 3; i++){
            HardwareOrder type = order_types[i];
            if (hardware_read_order(f, type)) {
                add_order(f, type);
                any_order = 1;
            }
        }
    }
    
    return any_order;
}

