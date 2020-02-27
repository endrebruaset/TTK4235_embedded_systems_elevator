/**
 * @file
 * @brief Module for timing events.
 */


#include <time.h>


#ifndef TIME_H
#define TIME_H


/**
 * @brief Data structure to be used to time events.
 */
typedef struct {
    time_t value; ///< The current time, valued in seconds.
    time_t trigger; ///< The limit the timer needs to reach to elapse, valued in seconds.
} Timer;


/**
 * @brief Sets the timer to @p seconds, by setting the timers value to the current time,
 * and the trigger @p seconds seconds higher than the timer value.
 * 
 * @param seconds The duration of the timer being set.
 */
void timer_set(int seconds);


/**
 * @brief Checks if the timer has elapsed, by checking if the current time @c Timer.value 
 * is larger than the the trigger @c Timer.trigger.
 * 
 * @return 1 if the timer has elapsed, 0 if not.
 */
int timer_is_elapsed();


#endif
