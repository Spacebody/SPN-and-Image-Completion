#include "utils.hpp"
#include "my_mpi.hpp"
#include <cmath>

//time
int Utils::get_int_val(const Instance &ins, const double p)
{
    return (int)(p * ins.std + ins.mean);
}

Timer Utils::timer = Timer();
void Utils::log_time(std::string &msg)
{
    long sec = Utils::timer.get_time_elapsed() / 1000;
    std::string str = std::string("<TIME> ") + msg + " " + std::to_string(sec) + std::string("s");
    Utils::println(msg);
    Utils::timer.timer_start();
}
void Utils::log_time_ms(std::string &msg)
{
    long sec = Utils::timer.get_time_elapsed();
    std::string str = std::string("<TIME> ") + msg + " " + std::to_string(sec) + std::string("ms");
    Utils::println(msg);
    Utils::timer.timer_start();
}

// logging
void Utils::print(const std::string &s)
{
    std::cout << Utils::get_prefix() + s << std::endl;
}

void Utils::println(const std::string &s)
{
    std::cout << s << std::endl;
}

void Utils::println()
{
    std::cout << std::endl;
}

std::string Utils::get_prefix()
{
    return std::string("[Rank=]") + std::to_string(MyMPI::rank) + std::string("]");
}

std::string Utils::left_pad(std::string s, const int len, const char c)
{
    for (int i = 0; i < len - s.length(); ++i)
    {
        s += c;
    }
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
    {
        k *= 10;
    }
    int y = (int)(x * k);
    return y * 1.0 / k;
}

double Utils::add_log(const double l1, const double l2)
{
    return l1 > l2 ? l1 + log(1 + exp(l2 - l1)) : l2 + log(1 + exp(l1 - l2));
}

// random
std::random_device Utils::rd;
std::default_random_engine Utils::random_(Utils::rd);

int Utils::random_next_int(int bound)
{
    std::uniform_int_distribution<int> dist(0, bound);
    return dist(Utils::random_);
}

