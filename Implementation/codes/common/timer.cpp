#include "timer.hpp"

clock_t Timer::start = 0;
clock_t Timer::end = 0;

void Timer::timer_start()
{
    Timer::start = clock();
}

long Timer::get_time_elapsed()
{
    Timer::end = clock();
    return Timer::end - Timer::start;
}
