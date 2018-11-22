#include "timer.hpp"

void Timer::timer_start()
{
    time(&start_timer);
}

long Timer::get_time_elapsed()
{
    static time_t end_timer;
    time(&end_timer);
    return (long)difftime(end_timer, start_timer);
}
