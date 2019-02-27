#ifndef IMAGE_COMPLETION_HPP
#define IMAGE_COMPLETION_HPP

#include "../common/utils.hpp"
#include "../common/parameter.hpp"
#include "../common/my_mpi.hpp"
#include "../spn/SPN.hpp"
#include "../spn/instance.hpp"
#include <fstream>

class ImageCompletion
{
    static int PAD_LEN;

    //-------------------------------------------------------------
    // Completion
    //-------------------------------------------------------------
    static void output_rst_to_img(std::ofstream &out, int inst_idx, Instance inst);
    static void send_img(int dest);
    static int recv_img(int src);

  public:
    static void complete_left(std::vector<Instance> test, std::string mdl_dir, std::string mdl_name, std::string rst_dir);
    static void complete_left(SPN spn, std::vector<Instance> test, std::string mdl_name, std::string rst_dir);
    static void complete_bottom(std::vector<Instance> test, std::string mdl_dir, std::string mdl_name, std::string rst_dir);
    static void complete_bottom(SPN spn, std::vector<Instance> test, std::string mdl_name, std::string rst_dir);
  
  private:
    // nothing
};

#endif
