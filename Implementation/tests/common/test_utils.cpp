#include <iostream>
#include "../../codes/common/utils.hpp"
#include "../../codes/spn/instance.hpp"

int main(int argc, char *argv[])
{
    Utils utils;
    Instance inst;
    std::cout << "Time interval: " << Utils::get_int_val(inst, 1.0) << std::endl;
    std::string log_msg = "Test";
    Utils::log_time(log_msg);
    Utils::log_time_ms(log_msg);
    std::cout << Utils::get_prefix() << std::endl;
    return 0;
}
