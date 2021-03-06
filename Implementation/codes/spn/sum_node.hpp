#ifndef SUM_NODE_HPP
#define SUM_NODE_HPP

#include "node.hpp"
#include <map>
#include "../common/parameter.hpp"
#include <memory>

class SumNode: public Node
{
  public:
    SumNode();
    ~SumNode() {}

    std::map<std::string, std::shared_ptr<Node> > children;
    std::map<std::string, double> child_cnts;
    double cnt;

    void eval();
    void pass_derivative();
    double get_child_cnt(std::string di);
    void set_child_cnt(std::string di, double cnt);
    void add_child_only(std::string decomp_idx, double cnt, std::shared_ptr<Node> n);
    void remove_child_only(std::string decomp_idx, double cnt);
};

#endif
