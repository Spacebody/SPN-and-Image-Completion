#ifndef TIMER_HPP
#define TIMER_HPP

#include <ctime>

class Timer
{
  private:
    static clock_t start, end;

  public:
    Timer() {}
    ~Timer() {}

    static void timer_start();      // start timer
    static long get_time_elapsed(); // get elapsed time
};

#endif
