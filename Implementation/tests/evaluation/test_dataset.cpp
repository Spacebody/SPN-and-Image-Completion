#include "../../codes/evaluation/dataset.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    Dataset ds;
    std::cout << "cal_mdl_dir: " << Dataset::cal_mdl_dir << std::endl;
    ds.load_olivetti();
    std::cout << "load_olivetti okay." << std::endl;
    ds.get_train();
    std::cout << "get_train okay." << std::endl;
    return 0;
}
