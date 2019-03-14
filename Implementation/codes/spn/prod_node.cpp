#include "prod_node.hpp"
#include "../common/utils.hpp"

ProdNode &ProdNode::operator=(const ProdNode &prod_node)
{
    if (this != &prod_node)
    {
        this->children = prod_node.children;
    }
    return *this;
}

ProdNode::ProdNode(const ProdNode &prod_node)
{
    if (this != &prod_node)
    {
        this->children = prod_node.children;
    }
}

void ProdNode::pass_derivative()
{
    if (this->get_log_derivative() == Node::zero_log_val) return;
    
    if (this->get_log_val() == Node::zero_log_val)
    {
        int cnt = 0;
        for (std::vector<Node>::iterator iter = this->children.begin();
             iter != this->children.end(); ++iter)
        {
            if (iter->get_log_val() == Node::zero_log_val)
            {
                ++cnt;
                if (cnt > 1) return;
            }
        }
    }

    for (std::vector<Node>::iterator iter1 = this->children.begin();
         iter1 != this->children.end(); ++iter1)
    {
        if (iter1->get_log_val() == Node::zero_log_val)
        {
            double l = 0;
            for (std::vector<Node>::iterator iter2 = this->children.begin();
                 iter2 != this->children.end(); ++iter2)
            {
                if (iter2->get_log_val() != Node::zero_log_val)
                    l += iter2->get_log_val();

                l += this->get_log_derivative();
                if (iter1->get_log_derivative() == Node::zero_log_val)
                    iter1->set_log_derivative(l);
                else
                    iter1->set_log_derivative(Utils::add_log(iter1->get_log_derivative(), l));
            }
        }
        else if (this->get_log_val() != Node::zero_log_val)
        {
            double l = this->get_log_derivative() + this->get_log_val() - iter1->get_log_val();
            if (iter1->get_log_derivative() == Node::zero_log_val)
                iter1->set_log_derivative(l);
            else
                iter1->set_log_derivative(Utils::add_log(iter1->get_log_derivative(), l));
        }
    }
}

void ProdNode::eval()
{
    this->set_log_val(0);
    for (std::vector<Node>::iterator iter = this->children.begin(); \
            iter != this->children.end(); ++iter)
    {
        double v = iter->get_log_val();
        if (v == Node::zero_log_val)
        {
            this->set_log_val(Node::zero_log_val);
            return;
        }
    }
}

void ProdNode::add_child(Node n)
{
    this->children.push_back(n);
}