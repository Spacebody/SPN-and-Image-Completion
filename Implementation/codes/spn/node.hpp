#ifndef _NODE_HPP
#define _NODE_HPP

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

    double log_val;
    double log_derivative;
    
    double get_log_val();
    void set_value(double v);

    /*
        the two pure virtual function should be implemented in subclasses
    */    
    // evaluate root
    virtual void eval();

    // propagate derivative to children
    virtual void pass_derivative();


   
};

#endif