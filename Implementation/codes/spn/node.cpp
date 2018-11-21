#include "node.hpp"
#include <limits>
#include <cmath>

double Node::zero_log_val = (std::numeric_limits<double>::min)();

double Node::get_log_val()
{
    return this->log_val;
}

void Node::set_value(double v)
{
    v == 0 ? this->log_val = Node::zero_log_val : this->log_val = log(v);
}