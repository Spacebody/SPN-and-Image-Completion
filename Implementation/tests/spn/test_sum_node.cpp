#include <iostream>
#include "../../codes/spn/sum_node.hpp"

int main(int argc, char *argv[])
{
    SumNode sum_node;
    std::cout << "Test eval, pass_derivative, set_child_cnt:" << std::endl;
    sum_node.eval();
    std::cout << "eval okay." << std::endl;
    sum_node.pass_derivative();
    std::cout << "pass_derivative okay." << std::endl;
    sum_node.set_child_cnt("Test", 1.0);
    std::cout << "set_child_cnt okay." << std::endl;
    return 0;
}
