#include "../../codes/evaluation/image_completion.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    ImageCompletion ic;
    std::cout << ic.get_pad_len() << std::endl;
    return 0;
}
