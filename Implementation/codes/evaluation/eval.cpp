#include "eval.hpp"
#include "../common/parameter.hpp"
#include <cmath>
#include <iostream>

std::string Eval::exp_dir = "../../../Implementation";

void Eval::eval_olivetti()
{
    int size = Parameter::input_dim1, pad_len = 10;
    std::string olive_dir = Eval::exp_dir + "/results/olivetti";
    double lm = -1;
    lm = Eval::cmp_MSE_left(olive_dir + "/completions/olive-left.dat", size, pad_len);
    double bm = -1;
    bm = Eval::cmp_MSE_bottom(olive_dir + "/completions/olive-btm.dat", size, pad_len);
    std::cout << "\n\nOlivetti MSE\t Left=" + std::to_string(round(lm)) + "\tBottom=" + std::to_string(round(bm)) << std::endl;
}

void Eval::eval_caltech()
{

}

double Eval::cmp_MSE_left(std::string fn, int size, int pad_len)
{

}

double Eval::cmp_MSE_bottom(std::string fn, int size, int pad_len)
{

}