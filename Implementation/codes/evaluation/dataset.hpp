#ifndef DATASET_HPP
#define DATASET_HPP

#include "../common/parameter.hpp"
#include "../spn/instance.hpp"
#include "../common/my_mpi.hpp"
#include <vector>
#include <string>
#include <set>

class Dataset
{
    /*  
        tmp
        dim1: Parameter::input_dim1,
        dim2: Parameter::input_dim2
    */
    static std::vector<std::vector<int> > tmp;  

    // data
    static std::string exp_dir;
    static std::string olivetti_raw_file_name;
    static std::string cal_data_dir;
    static std::string cal_rst_dir;
    static std::string cal_mdl_dir;
    
    static int RESCALE_LEN;

    std::vector<Instance> train;
    std::vector<Instance> test;

    // divide train/test
    static std::set<int> gen_test_idx(int max_size, int test_size);

    static void set_instance(std::vector<std::vector<int> > buf, Instance inst);

    //---------------------------------------------------
    // Caltech
    //---------------------------------------------------
    static Instance read_cal_instance(std::string fn);

    //---------------------------------------------------
    // Olivetti
    //---------------------------------------------------
    static Instance read_olivetti_instance(std::vector<std::vector<double> > faces, int pi);

  public:
    // dataset
    std::vector<Instance> get_train();
    std::vector<Instance> get_test();
    
    //---------------------------------------------------
    // Caltech
    //---------------------------------------------------
    void load_caltech(std::string dir_name);

    //---------------------------------------------------
    // Olivetti
    //---------------------------------------------------
    void load_olivetti();

  private:
    // nothing
};

#endif
