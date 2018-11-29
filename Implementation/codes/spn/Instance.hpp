#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <vector>

class Instance
{
  public:
    std::vector<std::vector<double> > vals;
    double mean;
    double std;

    Instance():mean(0), std(1)
    {
        this->vals.empty();
    }
    ~Instance(){};
    Instance(const Instance &ins);
    Instance &operator=(const Instance &ins);

};

#endif