#ifndef _PROD_NODE_HPP
#define _PROD_NODE_HPP

#include "node.hpp"
#include <vector>

class ProdNode : public Node
{
  public:
    ProdNode()
    {
        this->children.empty();
    }
    ~ProdNode() {}

    std::vector<Node> children;

    void pass_derivative();
    void eval();
    void add_child(Node n);

};

#endif