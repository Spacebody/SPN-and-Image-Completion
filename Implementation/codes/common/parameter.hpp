#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include <string>

class Parameter
{
  public:
    Parameter() {}
    ~Parameter() {}
    // Parameter(const Parameter &parameter);
    // Parameter &operator=(const Parameter &parameter);

    // EM
    static int max_iter;
    static double threshold_LLHChg;
    static int batch_size;
    static double sparse_prior;

    // SPN
    static int num_sum_per_region;
    static int input_dim1;
    static int input_dim2;
    static int base_resolution;
    static double smooth_sum_cnt;
    static int num_components_per_var;

    // Eval
    static int max_test_size;
    static std::string domain;
    static int num_slave_per_class;
    static int num_slave_grp;
};

#endif
