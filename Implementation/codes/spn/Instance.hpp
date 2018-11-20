#ifndef _INSTANCE_HPP
#define _INSTANCE_HPP

#include <vector>

class Instance
{
  public:
    std::vector<std::vector<double> > vals;
    double mean;
    double std;

    Instance();
    ~Instance(){};
};

#endif