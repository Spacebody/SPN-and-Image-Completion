#include "sum_node.hpp"
#include "node.hpp"
#include <cmath>
#include "../common/utils.hpp"

SumNode::SumNode()
{
    this->cnt = Parameter::smooth_sum_cnt;
    this->children = std::map<std::string, std::shared_ptr<Node> >();
    this->child_cnts = std::map<std::string, double>();
}

void SumNode::eval()
{
    double v = 0;
    std::string max_i = "";
    double max_l = 0;
    std::map<std::string, std::shared_ptr<Node> >::iterator iter;
    for (iter = this->children.begin(); iter != this->children.end(); ++iter)
    {
        double l = this->children[iter->first]->log_val;
        if (l == Node::zero_log_val)
            continue;
        if (max_i == "" || max_l < l)
        {
            max_i = iter->first;
            max_l = l;
        }
    }
    if (max_i == "")
    {
        this->log_val = Node::zero_log_val;
        return;
    }
    for (iter = this->children.begin(); iter != this->children.end(); ++iter)
    {
        if(this->children.count(iter->first) == 0)
            continue;
        double l = this->children[iter->first]->log_val;
        if(l == Node::zero_log_val)
            continue;
        v += this->get_child_cnt(iter->first) * exp(l - max_l);
    }
    this->log_val = log(v / this->cnt) + max_l;
}

void SumNode::pass_derivative()
{
    if (this->log_derivative == Node::zero_log_val)
        return;
    std::map<std::string, std::shared_ptr<Node> >::iterator iter;
    for (iter = this->children.begin(); iter != this->children.end(); ++iter)
    {
        Node &n = *(this->children[iter->first]);
        double l = this->log_derivative + log(this->get_child_cnt(iter->first) / this->cnt);
        if (n.log_derivative == Node::zero_log_val)
            n.log_derivative = l;
        else
            n.log_derivative = Utils::add_log(l, n.log_derivative);
    }
}

double SumNode::get_child_cnt(std::string di)
{
    return this->child_cnts[di];
}

void SumNode::set_child_cnt(std::string di, double cnt_)
{
    this->child_cnts[di] = cnt_;
}

void SumNode::add_child_only(std::string decomp_idx, double cnt_, std::shared_ptr<Node> n)
{
    if (this->children.count(decomp_idx) == 0)
        this->children.insert(std::make_pair(decomp_idx, n));
    if (this->child_cnts.count(decomp_idx) == 0)
        this->child_cnts.insert(std::make_pair(decomp_idx, cnt_));
    else
        this->child_cnts[decomp_idx] = cnt_ + this->get_child_cnt(decomp_idx);
    this->cnt += cnt_;
}

void SumNode::remove_child_only(std::string decomp_idx, double cnt_)
{
    double child_cnt = this->get_child_cnt(decomp_idx);
    child_cnt -= cnt_;
    if (child_cnt == 0)
    {
        this->children.erase(decomp_idx);
        this->child_cnts.erase(decomp_idx);
    }
    else
        this->child_cnts[decomp_idx] = child_cnt;
    this->cnt -= cnt_;
}
