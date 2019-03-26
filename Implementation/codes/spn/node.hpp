#ifndef NODE_HPP
#define NODE_HPP

// base class of SPN node
class Node
{
  public:
    Node();
    virtual ~Node() {}

    static double zero_log_val;
    double log_val;
    double log_derivative;

    void set_log_val(double value);

    /*
        the two pure virtual function should be implemented in subclasses
    */    
    // evaluate root
    virtual void eval();

    // propagate derivative to children
    virtual void pass_derivative();
};

#endif