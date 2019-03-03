#include "../../codes/common/timer.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    Timer::timer_start();
    for (int i = 0; i < 99999999; ++i) {}
    std::cout << Timer::get_time_elapsed() << std::endl;
    return 0;
}
