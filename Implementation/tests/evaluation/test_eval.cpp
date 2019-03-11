#include "../../codes/evaluation/eval.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    Eval eval;
    std::cout << eval.get_exp_dir() << std::endl;
    Eval::eval_olivetti();
    std::cout << "eva_olivetti okay." << std::endl;
    return 0;
}
