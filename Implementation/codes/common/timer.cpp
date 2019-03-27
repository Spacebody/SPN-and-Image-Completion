#include "timer.hpp"

clock_t Timer::start_time = 0;

Timer::Timer()
{
    Timer::timer_start();
}

void Timer::timer_start()
{
    Timer::start_time = clock();
}

long Timer::get_time_elapsed()
{
    return clock() - Timer::start_time;
}
