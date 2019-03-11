#include "run.hpp"
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI::Init(argc, argv);
    Run run;
    run.proc(argc, argv);
    return 0;
}
