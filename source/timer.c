#include "timer.h"


void timer_set(int seconds) {
    time(&timer.value);
    timer.trigger = timer.value + seconds;
}


int timer_is_elapsed() {
    time(&timer.value);
    return (timer.value >= timer.trigger);
}