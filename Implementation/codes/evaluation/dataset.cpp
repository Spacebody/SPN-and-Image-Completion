#include "dataset.hpp"

std::string Dataset::exp_dir = "../..";
std::string Dataset::olivetti_raw_file_name = Dataset::exp_dir + "/data/olivetti/olivetti.raw";
std::string Dataset::cal_data_dir = Dataset::exp_dir + "/data/caltech";
std::string Dataset::cal_rst_dir = Dataset::exp_dir + "/results/caltech/completions";
std::string Dataset::cal_mdl_dir = Dataset::exp_dir +"/results/caltech/models";

int Dataset::RESCALE_LEN = 100;


// dataset
std::vector<Instance> Dataset::get_train()
{
    return this->train;
}

std::vector<Instance> Dataset::get_test()
{
    return this->test;
}

