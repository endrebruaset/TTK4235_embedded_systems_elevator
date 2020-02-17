/**
 * @file
 * @brief 
 */


#include <time.h>


#ifndef TIME_H
#define TIME_H


/**
 * @brief Data structure to be used to time events. Data member @c value represents the current time.
 */
typedef struct {
    time_t value;
    time_t trigger;
} Timer;


/**
 * @brief Sets @p timer to @p seconds, by setting the timers value to the current timer, \
 * and the trigger @p second higher than the timer value.
 * 
 * @param timer The @c Timer object to set.
 * 
 * @param seconds The duration of the timer being set.
 */
void timer_set(Timer timer, int seconds);


/**
 * @brief Checks if the timer has elapsed, by checking if the current timer value is larger than the the trigger.
 * 
 * @param timer The timer object to be checked.
 * 
 * @return 1 if the timer has elapsed, 0 if not.
 */
int timer_is_elapsed(Timer timer);


#endif
