#include "prod_node.hpp"
#include "../common/utils.hpp"

ProdNode::ProdNode()
{
    this->children = std::vector<Node>();
}

void ProdNode::pass_derivative()
{
    if (this->log_derivative == Node::zero_log_val) return;
    
    if (this->log_val == Node::zero_log_val)
    {
        int cnt = 0;
        for (std::vector<Node>::iterator iter = this->children.begin();
             iter != this->children.end(); ++iter)
        {
            if (iter->log_val == Node::zero_log_val)
            {
                ++cnt;
                if (cnt > 1) return;
            }
        }
    }

    for (std::vector<Node>::iterator iter1 = this->children.begin();
         iter1 != this->children.end(); ++iter1)
    {
        if (iter1->log_val == Node::zero_log_val)
        {
            double l = 0;
            for (std::vector<Node>::iterator iter2 = this->children.begin();
                 iter2 != this->children.end(); ++iter2)
            {
                if (iter2->log_val != Node::zero_log_val)
                    l += iter2->log_val;

                l += this->log_derivative;
                if (iter1->log_derivative == Node::zero_log_val)
                    iter1->log_derivative = l;
                else
                    iter1->log_derivative = Utils::add_log(iter1->log_derivative, l);
            }
        }
        else if (this->log_val != Node::zero_log_val)
        {
            double l = this->log_derivative + this->log_val - iter1->log_val;
            if (iter1->log_derivative == Node::zero_log_val)
                iter1->log_derivative = l;
            else
                iter1->log_derivative = Utils::add_log(iter1->log_derivative, l);
        }
    }
}

void ProdNode::eval()
{
    this->log_val = 0;
    for (std::vector<Node>::iterator iter = this->children.begin();
         iter != this->children.end(); ++iter)
    {
        double v = iter->log_val;
        if (v == Node::zero_log_val)
        {
            this->log_val = Node::zero_log_val;
            return;
        }
        this->log_val += v;
    }
}

void ProdNode::add_child(Node &n)
{
    this->children.push_back(n);
}
