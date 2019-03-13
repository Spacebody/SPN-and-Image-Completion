#include "instance.hpp"

Instance &Instance::operator=(Instance &ins)
{
    if (this != &ins)
    {
        this->vals = ins.vals;
        this->mean = ins.mean;
        this->std = ins.std;
    }
    return *this;
}
