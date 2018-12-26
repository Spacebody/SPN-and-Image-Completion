#include "sum_node.hpp"
#include "node.hpp"
#include <cmath>
#include "../common/utils.hpp"

void SumNode::eval()
{
    double v = 0;
    std::string max_i = "";
    double max_l = 0;
    std::map<std::string, Node>::iterator iter;
    for (iter = this->children.begin(); iter != this->children.end(); ++iter)
    {
        double l = this->children[iter->first].get_log_val();
        if (l == Node::zero_log_val)
            continue;
        if (max_i == "" || max_l == l)
        {
            max_i = iter->first;
            max_l = l;
        }
    }
    if (max_i == "")
    {
        SumNode::set_log_val(Node::zero_log_val);
        return;
    }
    for (iter = this->children.begin(); iter != this->children.end(); ++iter)
    {
        if(!this->children.count(iter->first))
            continue;
        double l = this->children[iter->first].get_log_val();
        if(l == Node::zero_log_val)
            continue;
        v += SumNode::get_child_cnt(iter->first) * exp(l - max_l);
    }
    SumNode::set_log_val(log(v / this->cnt) + max_l);
}

void SumNode::pass_derivative()
{
    if (SumNode::get_log_derivative() == Node::zero_log_val)
        return;
    std::map<std::string, Node>::iterator iter;
    for (iter = this->children.begin(); iter != this->children.end(); ++iter)
    {
        Node n = this->children[iter->first];
        double l = SumNode::get_log_derivative() + log(SumNode::get_child_cnt(iter->first) / this->cnt);
        if (n.get_log_derivative() == Node::zero_log_val)
            n.set_log_derivative(l);
        else
            n.set_log_derivative(Utils::add_log(l, n.get_log_derivative()));
    }
}

double SumNode::get_child_cnt(std::string di)
{
    return this->child_cnts[di];
}

void SumNode::set_child_cnt(std::string di, double cnt)
{
    this->child_cnts[di] = cnt;
}

void SumNode::add_child_only(std::string decomp_idx, double cnt, Node n)
{
    if (!this->children.count(decomp_idx))
        this->children.insert(std::pair<std::string, Node>(decomp_idx, n));
    if (!this->child_cnts.count(decomp_idx))
        this->child_cnts.insert(std::pair<std::string, double>(decomp_idx, cnt));
    else
        this->child_cnts.insert(std::pair<std::string, double>(decomp_idx, cnt + SumNode::get_child_cnt(decomp_idx)));
    this->cnt = this->cnt + cnt;
}

void SumNode::remove_child_only(std::string decomp_idx, double cnt)
{
    double child_cnt = SumNode::get_child_cnt(decomp_idx);
    child_cnt -= cnt;
    std::map<std::string, Node>::iterator iter_child;
    std::map<std::string, double>::iterator iter_child_cnt;
    if (child_cnt == 0)
    {
        iter_child = this->children.find(decomp_idx);
        this->children.erase(iter_child);
        iter_child_cnt = this->child_cnts.find(decomp_idx);
        this->child_cnts.erase(iter_child_cnt);
    }
    else
        this->child_cnts.insert(std::pair<std::string, double>(decomp_idx, cnt));
    this->cnt -= cnt;
}

std::map<std::string, Node> SumNode::get_children()
{
    return this->children;
}

double SumNode::get_cnt()
{
    return this->cnt;
}

void SumNode::add_child_only(std::string di, Node n)
{
    this->children.insert(std::pair<std::string, Node>(di, n));
}
