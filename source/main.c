/**
 * @file
 * @brief
 */


#include <signal.h>
#include "fsm.h"


static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

int main(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    signal(SIGINT, sigint_handler);


    fsm_initialize();
    
    
    while(1) {
        switch (m_current_state)
        {
            case MOVING:
            {
                fsm_in_state_moving();
                break;
            }

            case STAYING:
            {
                fsm_in_state_staying();
                break;
            }

            case IDLE:
            {
                fsm_in_state_idle();
                break;
            }

            case EMERGENCY_STOP:
            {
                fsm_in_state_emergency_stop();
                break;
            }

            default:
                fprintf(stderr, "Undefined state");
                exit(2);
                break;
        }
    }

    return 0;
}
