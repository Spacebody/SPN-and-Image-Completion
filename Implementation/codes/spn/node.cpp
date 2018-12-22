#include "node.hpp"
#include <limits>
#include <cmath>

double Node::zero_log_val = (std::numeric_limits<double>::min)();

double Node::get_log_val()
{
    return this->log_val;
}

double Node::get_log_derivative()
{
    return this->log_derivative;
}

void Node::set_log_val(double value)
{
    value == 0 ? this->log_val = Node::zero_log_val : this->log_val = log(value);
}

void Node::set_log_derivative(double derivative)
{
    this->log_derivative = derivative;
}