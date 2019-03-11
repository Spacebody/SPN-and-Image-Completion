#ifndef EVAL_HPP
#define EVAL_HPP

#include <string>

class Eval
{
    static std::string exp_dir;

    static double cmp_MSE_left(std::string fn, int size, int pad_len);
    static double cmp_MSE_bottom(std::string fn, int size, int pad_len);

  public:
    static void eval_olivetti();
    static void eval_caltech();

    std::string get_exp_dir();  // just for testing
};

#endif