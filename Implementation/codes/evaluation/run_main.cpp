#include "run.hpp"
#include <mpi.h>

int main(int argc, char *argv[])
{
    // MPI::Init(argc, argv);  // initialize MPI environment
    MPI_Init(&argc, &argv);
    Run run;
    run.proc(argc, argv);
    // MPI::Finalize();  // clean MPI environment
    MPI_Finalize();
    return 0;
}
