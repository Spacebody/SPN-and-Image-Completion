#include <iostream>
#include "../../codes/common/my_mpi.hpp"

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    MyMPI my_mpi;
    std::cout << MyMPI::rank << std::endl;
    my_mpi.~MyMPI();
    return 0;
}
