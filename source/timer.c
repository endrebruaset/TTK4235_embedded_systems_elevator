#include "timer.h"


void timer_set(int seconds) {
    time(&m_timer.value);
    m_timer.trigger = m_timer.value + seconds;
}


int timer_is_elapsed() {
    time(&m_timer.value);
    return (m_timer.value >= m_timer.trigger);
}