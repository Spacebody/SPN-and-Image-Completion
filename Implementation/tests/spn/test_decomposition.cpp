#include <iostream>
#include "../../codes/spn/decomposition.hpp"

int main(int argc, char *argv[])
{
    Decomposition decomp("Test", 1, 2, 1, 2);
    std::cout << decomp.get_id() << std::endl;
    std::cout << "get_id okay." << std::endl;
    Decomposition::blank_decomp.get_id();
    std::cout << "get_blank_decomp okay." << std::endl;
    std::cout << Decomposition::get_decomposition(1, 2, 1, 2).get_id() << std::endl;
    std::cout << "get_decomposition(ri1, ri2, type1, type2) okay." << std::endl;
    std::cout << Decomposition::get_decomposition("Test 1 2 1 2").get_id() << std::endl;
    std::cout << "get_decomposition(id) okay." << std::endl;
    std::cout << Decomposition::get_id_str(1, 2, 1, 2) << std::endl;
    std::cout << "get_id_str okay." << std::endl;
    return 0;
}
