/**
 * @file
 * @brief
 */


#include "timer.h"


void timer_set(int seconds) {
    timer.value = time(NULL);
    timer.trigger = timer.value + seconds;
}


int timer_is_elapsed() {
    timer.value = time(NULL);
    return (timer.value >= timer.trigger);
}