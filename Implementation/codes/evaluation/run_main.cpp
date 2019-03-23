#include "run.hpp"
#include <mpi.h>
#include "../common/my_mpi.hpp"

int main(int argc, char *argv[])
{
    MPI::Init(argc, argv);  // initialize MPI environment
    MyMPI::rank = MPI::COMM_WORLD.Get_rank();  // set rank
    Run run;
    run.proc(argc, argv);
    MPI::Finalize();  // clean MPI environment
    return 0;
}
