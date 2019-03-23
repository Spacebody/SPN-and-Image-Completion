#include "run.hpp"
#include <mpi.h>
#include "../common/my_mpi.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    MPI_Init(NULL, NULL); // initialize MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &MyMPI::rank);  // set rank
    std::cout << "My rank: " << MyMPI::rank << std::endl;
    Run run;
    run.proc(argc, argv);
    MPI_Finalize();  // clean MPI environment
    return 0;
}
