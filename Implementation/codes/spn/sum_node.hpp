#ifndef SUM_NODE_HPP
#define SUM_NODE_HPP

#include "node.hpp"
#include <map>
#include "../common/parameter.hpp"

class SumNode: public Node
{
  public:
    SumNode() : cnt(Parameter::smooth_sum_cnt)
    {
        this->children.empty();
        this->child_cnts.empty();
    }
    ~SumNode() {}
    SumNode(const SumNode &sum_node);
    SumNode &operator=(const SumNode &sum_node);

    void eval();
    void pass_derivative();
    double get_child_cnt(std::string di);
    void set_child_cnt(std::string di, double cnt);
    void add_child_only(std::string decomp_idx, double cnt, Node n);
    void remove_child_only(std::string decomp_idx, double cnt);

    std::map<std::string, Node> get_children();
    double get_cnt();

  private:
    std::map<std::string, Node> children;
    std::map<std::string, double> child_cnts;
    double cnt;
};

#endif