#include "parameter.hpp"

int Parameter::max_iter = 30;
double Parameter::threshold_LLHChg = 0.1;
int Parameter::batch_size = 2;  //50
double Parameter::sparse_prior = 1;

int Parameter::num_sum_per_region = 20;
int Parameter::input_dim1 = 100;
int Parameter::input_dim2 = 64;
int Parameter::base_resolution = 4;
double Parameter::smooth_sum_cnt = 0.01;
int Parameter::num_components_per_var = 4;

int Parameter::max_test_size = 10; // 50
std::string Parameter::domain = "";
int Parameter::num_slave_per_class = 2;  //50
int Parameter::num_slave_grp = -1;
