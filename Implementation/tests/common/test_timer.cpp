#include "../../codes/common/timer.hpp"
#include <iostream>
#include <cmath>

int frequency_of_primes(int n)
{
    int i, j;
    int freq = n - 1;
    for (i = 2; i <= n; ++i)
        for (j = sqrt(i); j > 1; --j)
            if (i % j == 0)
            {
                --freq;
                break;
            }
    return freq;
}


int main(int argc, char *argv[])
{
    std::cout << "Calculate factorial number..." << std::endl;
    Timer::timer_start();
    frequency_of_primes(99999);
    std::cout << "Total spend time: " << Timer::get_time_elapsed() << std::endl;
    return 0;
}
