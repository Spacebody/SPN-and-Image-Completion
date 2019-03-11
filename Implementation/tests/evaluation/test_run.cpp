#include "../../codes/evaluation/run.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    Run run;
    std::cout << Run::exp_dir << std::endl;
    std::cout << Run::olive_rst_dir << std::endl;
    std::cout << Run::olive_mdl_dir << std::endl;
    std::cout << Run::cal_data_dir << std::endl;
    std::cout << Run::DOM_CALTECH << std::endl;
    std::cout << Run::DOM_OLIVETTI << std::endl;
    return 0;
}
