#ifndef NODE_HPP
#define NODE_HPP

// base class of SPN node
class Node
{
  public:
    Node()
    {
        this->log_derivative = Node::zero_log_val;
    }
    virtual ~Node() {}
    Node(const Node &node);
    Node &operator=(const Node &node);

    static double zero_log_val;
    
    double get_log_val();
    double get_log_derivative();
    void set_log_val(double value);
    void set_log_derivative(double derivative);

    /*
        the two pure virtual function should be implemented in subclasses
    */    
    // evaluate root
    virtual void eval();

    // propagate derivative to children
    virtual void pass_derivative();

  private:
    double log_val;
    double log_derivative;
};

#endif