#include "run.hpp"
#include <mpi.h>
#include <iostream>

int main(int argc, char *argv[])
{
    MPI_Init(NULL, NULL);
    Run run;
    run.proc(argc, argv);
    MPI_Finalize();
    return 0;
}
