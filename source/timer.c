/**
 * @file
 * @brief
 */


#include "timer.h"


void timer_set(Timer timer, int seconds) {
    timer.value = time(NULL);
    timer.trigger = timer.value + seconds;
}


int timer_is_elapsed(Timer timer) {
    timer.value = time(NULL);
    return (timer.value >= timer.trigger);
}