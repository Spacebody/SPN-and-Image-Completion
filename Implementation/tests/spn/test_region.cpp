#include <iostream>
#include "../../codes/spn/region.hpp"

int main(int argc, char *argv[])
{
    Region region;
    std::cout << "default constructor okay." << std::endl;
    Region region2(0, 1, 2, 3, 4);
    std::cout << "id, a1, a, interval: " << region.id << " " << region.a1 << " " << region.a << " " << region.interval << std::endl;
    std::cout << "get_id: " << region2.get_id() << std::endl;
    std::cout << "my_str: " << region2.my_str() << std::endl;
    std::cout << "cmp_Gauss: " << region2.cmp_Gauss(1, 0) << std::endl;
    return 0;
}
