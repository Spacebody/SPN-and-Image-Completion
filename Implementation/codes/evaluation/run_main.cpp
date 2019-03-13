// #include "../common/my_mpi.hpp"
#include "run.hpp"
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI::Init(argc, argv);  // initialize MPI environment
    Run run;
    run.proc(argc, argv);
    MPI::Finalize();  // clean MPI environment
    return 0;
}

