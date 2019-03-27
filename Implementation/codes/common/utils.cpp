#include "utils.hpp"
#include "my_mpi.hpp"
#include <cmath>

//time
int Utils::get_int_val(const Instance &inst, const double p)
{
    return (int)(p * inst.std + inst.mean);
}

Timer Utils::timer = Timer();
void Utils::log_time(std::string msg)
{
    long sec = Utils::timer.get_time_elapsed() / 1000;
    Utils::println(std::string("<TIME> ") + msg + " " + std::to_string(sec) + std::string("s"));
    Utils::timer.timer_start();
}
void Utils::log_time_ms(std::string msg)
{
    long sec = Utils::timer.get_time_elapsed();
    Utils::println(std::string("<TIME> ") + msg + " " + std::to_string(sec) + std::string("ms"));
    Utils::timer.timer_start();
}

// logging
void Utils::print(const std::string s)
{
    std::cout << Utils::get_prefix() + s << std::endl;
}

void Utils::println(const std::string s)
{
    std::cout << s << std::endl;
}

void Utils::println()
{
    std::cout << std::endl;
}

std::string Utils::get_prefix()
{
    return "[Rank=" + std::to_string(MyMPI::rank) + "] ";
}

std::string Utils::left_pad(std::string s, const int len, const char c)
{
    for (int i = 0; i < len - s.length(); ++i)
        s = c + s;
    return s;
}

// numeric
double Utils::round(const double x)
{
    return Utils::round(x, 2);
}
double Utils::round(const double x, const int n)
{
    double k = 1.0;
    for (int i = 0; i < n; ++i)
        k *= 10;
    int y = (int)(x * k);
    return y * 1.0 / k;
}

double Utils::add_log(const double l1, const double l2)
{
    return l1 > l2 ? l1 + log(1 + exp(l2 - l1)) : l2 + log(1 + exp(l1 - l2));
}

// random
long Utils::seed = -1; // reproducible; different
std::default_random_engine Utils::generator(Utils::seed);

int Utils::random_next_int(int bound)
{
    std::uniform_int_distribution<int> dist(0, bound);
    return dist(Utils::generator);
}

// trim string
std::string Utils::trim(std::string str)
{
    size_t first = str.find_first_not_of(' ');
    while (std::string::npos == first)
    {
        return "";
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}
