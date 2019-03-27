#include "node.hpp"
#include <limits>
#include <cmath>

double Node::zero_log_val = (std::numeric_limits<double>::min)();

Node::Node()
{
    this->log_derivative = Node::zero_log_val;
}

void Node::set_log_val(double value)
{
    value == 0 ? this->log_val = Node::zero_log_val : this->log_val = log(value);
}

// implement for testing
void Node::eval() {}
void Node::pass_derivative() {}
