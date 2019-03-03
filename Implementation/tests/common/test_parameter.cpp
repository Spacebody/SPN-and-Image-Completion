#include "../../codes/common/parameter.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "max iter: " << Parameter::max_iter << std::endl;
    std::cout << "domain: " << Parameter::domain << std::endl;
    std::cout << "smooth sum cnt: " << Parameter::smooth_sum_cnt << std::endl;
    return 0;
}
