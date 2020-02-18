/**
 * @file
 * @brief 
 */


#include <time.h>


#ifndef TIME_H
#define TIME_H
#define DEFAULT_TIME_DOOR_OPEN 3

/**
 * @brief Data structure to be used to time events. Data member @c value represents the current time.
 */
typedef struct {
    time_t value;
    time_t trigger;
} Timer;


/**
 * @brief Timer to be used to time events.
 */
Timer timer;


/**
 * @brief Sets the timer to @p seconds, by setting the timers value to the current time, \
 * and the trigger @p second higher than the timer value.
 * 
 * @param seconds The duration of the timer being set.
 */
void timer_set(int seconds);


/**
 * @brief Checks if the timer has elapsed, by checking if the current timer value is larger than the the trigger.
 * 
 * @return 1 if the timer has elapsed, 0 if not.
 */
int timer_is_elapsed();


#endif
