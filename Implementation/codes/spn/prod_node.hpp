#ifndef PROD_NODE_HPP
#define PROD_NODE_HPP

#include "node.hpp"
#include <vector>

class ProdNode : public Node
{
  public:
    ProdNode();
    ~ProdNode() {}

    void pass_derivative();
    void eval();
    void add_child(Node &n);

  private:
    std::vector<Node> children;
};

#endif
