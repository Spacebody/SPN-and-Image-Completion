#include "instance.hpp"

Instance::Instance()
{
    this->mean = 0;
    this->std = 1;
    this->vals = std::vector<std::vector<double> >();
}
