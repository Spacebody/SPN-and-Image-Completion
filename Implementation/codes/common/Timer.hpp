#ifndef _TIMER_HPP
#define _TIMER_HPP

#include <ctime>

class Timer
{
  private:
    static time_t start_timer;

  public:
    Timer() {}
    ~Timer() {}
    static void timer_start();  // start timer
    static long get_time_elapsed();  // get elapsed time
};

#endif