#include <iostream>
#include "../../codes/spn/node.hpp"

int main(int argc, char *argv[])
{
    Node node;
    std::cout << "set log value: 1.0" << std::endl;
    node.set_log_val(1.0);
    std::cout << "log value " << node.log_val << std::endl;
    std::cout << "set log derivative: 2.0" << std::endl;
    node.set_log_derivative(2.0);
    std::cout << "log derivative: " << node.log_derivative << std::endl;
    return 0;
}
