#ifndef GENERATIVE_LEARNING_HPP
#define GENERATIVE_LEARNING_HPP

#include "SPN.hpp"
#include "instance.hpp"
#include <vector>
#include <string>

class GenerativeLearning
{
  public:
    GenerativeLearning() : spn(SPN()) {}
    ~GenerativeLearning() {}

    SPN get_DSPN();
    void learn(std::vector<Instance> train);
    void save_model(std::string mdl_file_name);
    void learn_hard_EM(std::vector<Instance> train);

    //--------------------------------------------------
    // Utils
    //--------------------------------------------------
    void send_msg_break(int dest);
    void send_msg_OK(int dest);
    char recv_msg(int src);
    void send_llh(int dest, double d);
    double recv_llh(int src);

  private:
    SPN spn;

};

#endif