#include <iostream>
#include "../../codes/common/utils.hpp"
#include "../../codes/spn/instance.hpp"

int main(int argc, char *argv[])
{
    Utils utils;
    Instance inst;
    std::cout << "Time interval: " << Utils::get_int_val(inst, 1.0) << std::endl;
    Utils::log_time("Test");
    Utils::log_time_ms("Test");
    std::cout << Utils::get_prefix() << std::endl;
    return 0;
}
