#include <iostream>
#include "../../codes/spn/SPN.hpp"

int main(int argc, char *argv[])
{
    SPN dspn;
    std::cout <<"is_recording_update: " << dspn.is_recording_update << std::endl;
    std::cout << "coarse_dim1: " << dspn.coarse_dim1 << std::endl;
    // dspn.init();
    // std::cout << "init okay." << std::endl;
    dspn.clear_unused_in_SPN();
    std::cout << "clear_unused_in_SPN okay." << std::endl;
    dspn.print_params();
    std::cout << "print_params okay." << std::endl;
    return 0;
}
