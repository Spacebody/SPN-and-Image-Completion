#include "eval.hpp"

int main(int argc, char *argv[])
{
    Eval eval;
    eval.eval_caltech();
    eval.eval_olivetti();
    return 0;
}
