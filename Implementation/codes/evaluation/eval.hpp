#ifndef EVAL_HPP
#define EVAL_HPP

#include <string>

class Eval
{
    static std::string exp_dir;

    static void eval_olivetti();
    static void eval_caltech();

    static double cmp_MSE_left(std::string fn, int size, int pad_len);
    static double cmp_MSE_bottom(std::string fn, int size, int pad_len);
    
};

#endif