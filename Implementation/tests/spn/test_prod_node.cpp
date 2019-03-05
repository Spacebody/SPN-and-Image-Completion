#include <iostream>
#include "../../codes/spn/prod_node.hpp"
#include "../../codes/spn/node.hpp"

int main(int argc, char *argv[])
{
    ProdNode prod_node;
    std::cout << "Test eval, pass_derivative, add_child:" << std::endl;
    prod_node.pass_derivative();
    std::cout << "pass_derivative okay." << std::endl;
    prod_node.eval();
    std::cout << "eval okay." << std::endl;
    Node node;
    prod_node.add_child(node);
    std::cout << "add_child okay." << std::endl;
    return 0;
}

