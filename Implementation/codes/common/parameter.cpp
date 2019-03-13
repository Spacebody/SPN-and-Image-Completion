#include "parameter.hpp"

int Parameter::max_iter = 30;
double Parameter::threshold_LLHChg = 0.1;
int Parameter::batch_size = 50;
double Parameter::sparse_prior = 1;

int Parameter::num_sum_per_region = 20;
int Parameter::input_dim1 = 64;
int Parameter::input_dim2 = 64;
int Parameter::base_resolution = 4;
double Parameter::smooth_sum_cnt = 0.01;
int Parameter::num_components_per_var = 4;

int Parameter::max_test_size = 50;
std::string Parameter::domain = "";
int Parameter::num_slave_per_class = 50;
int Parameter::num_slave_grp = -1;

Parameter &Parameter::operator=(const Parameter &parameter)
{
    if (this != &parameter)
    {
        this->max_iter = parameter.max_iter;
        this->threshold_LLHChg = parameter.threshold_LLHChg;
        this->batch_size = parameter.batch_size;
        this->sparse_prior = parameter.sparse_prior;

        this->num_sum_per_region = parameter.num_sum_per_region;
        this->input_dim1 = parameter.input_dim1;
        this->input_dim2 = parameter.input_dim2;
        this->base_resolution = parameter.base_resolution;
        this->smooth_sum_cnt = parameter.smooth_sum_cnt;
        this->num_components_per_var = parameter.num_components_per_var;

        this->max_test_size = parameter.max_test_size;
        this->domain = parameter.domain;
        this->num_slave_per_class = parameter.num_slave_per_class;
        this->num_slave_grp = parameter.num_slave_grp;
    }
    return *this;
}
