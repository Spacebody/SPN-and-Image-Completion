#ifndef _SUM_NODE_HPP
#define _SUM_NODE_HPP

#include "node.hpp"
#include <map>
#include "../common/parameter.hpp"

class SumNode: public Node
{
    public:
        std::map<std::string, Node> children;
        std::map<std::string, double> child_cnts;
        double cnt;

        SumNode()
        {
            this->children.empty();
            this->child_cnts.empty();
            this->cnt = Parameter::smooth_sum_cnt;
        }
        ~SumNode() {}

        void eval();
        void pass_derivative();
        double get_child_cnt(std::string di);
        void set_child_cnt(std::string di, double cnt);
        void add_child_only(std::string, double cnt, Node n);
        void remove_child_only(std::string deomp_idx, double cnt);
};

#endif