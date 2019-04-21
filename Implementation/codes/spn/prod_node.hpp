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
    void add_child(std::shared_ptr<Node> n);

  private:
    std::vector<std::shared_ptr<Node> > children;
};

#endif
