#include <iostream>
#include "../../codes/common/my_mpi.hpp"

int main(int argc, char *argv[])
{
    MyMPI my_mpi(argc, argv);
    std::cout << MyMPI::rank << std::endl;
    return 0;
}
