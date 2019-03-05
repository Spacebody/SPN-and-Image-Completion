#include <iostream>
#include "../../codes/spn/instance.hpp"

int main(int argc, char *argv[])
{
    Instance inst;
    std::cout << "Instance mean: " << inst.mean << std::endl;
    std::cout << "Instance var: " << inst.std << std::endl;
    return 0;
}