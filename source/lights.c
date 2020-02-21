#include "lights.h"


void lights_clear_all_order_lights(){
    HardwareOrder order_types[HARDWARE_NUMBER_OF_ORDER_TYPES] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++){
            HardwareOrder type = order_types[i];
            hardware_command_order_light(f, type, 0);
        }
    }
}
