#include "../../codes/spn/generative_learning.hpp"
#include "../../codes/common/my_mpi.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    GenerativeLearning gl;
    std::cout << "coarse_dim1: " << gl.get_DSPN().coarse_dim1 << std::endl;
    MyMPI my_mpi(argc, argv);
    gl.send_msg_OK(0);
    std::cout << "send_msg_OK okay." << std::endl;
    std::cout << gl.recv_msg(0) << std::endl;
    gl.send_llh(0, 0.5);
    std::cout << "send_llh okay." << std::endl;
    std::cout << gl.recv_llh(0) << std::endl;
    return 0;
}
