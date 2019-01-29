#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <random>
#include "../spn/instance.hpp"
#include "timer.hpp"

class Utils
{
  public:
    Utils()
    {
        timer.timer_start();
    }
    ~Utils() {}
    Utils(const Utils &utils);
    Utils &operator=(const Utils &utils);

    // time
    static int get_int_val(const Instance &ins, const double p);

    static Timer timer;
    static void log_time(std::string &msg);
    static void log_time_ms(std::string &msg);

    // logging
    static void print(const std::string &s);
    static void println(const std::string &s);
    static void println();
    static std::string get_prefix();
    static std::string left_pad(std::string s, const int len, const char c);

    // numeric
    static double round(const double x);
    static double round(const double x, int n);
    static double add_log(const double l1, const double l2);

    // random
    static long seed;
    static std::random_device rd;
    static std::default_random_engine random_;
    static int random_next_int(int bound);
};

#endif