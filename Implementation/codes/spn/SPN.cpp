#include "SPN.hpp"

#include "../common/utils.hpp"

bool SPN::is_recording_update = true;
bool SPN::complete_by_marginal = true;

// TODO: implement the functions

// ----------------------------------------------
// Bottom
// ----------------------------------------------
// void SPN::complete_bottom_img(Instance inst);
// void SPN::cmp_MAP_bottom_half(Instance inst);
// void SPN::cmp_MAP_bottom_half_marginal(Instance inst);
// void SPN::infer_MAP_bottom_half(int ii, Instance inst);
// void SPN::set_MAP_bottom_to_buf(int inst_idx, Instance inst);

// ----------------------------------------------
// Left
// ----------------------------------------------
// void SPN::complete_left_img(Instance inst);
// void SPN::cmp_MAP_left_half(Instance inst);
// void SPN::cmp_MAP_left_half_marginal(Instance inst);
// double SPN::cmp_marginal(Region r);
// void SPN::set_MAP_left_to_buf(int inst_idx, Instance inst);

// ----------------------------------------------
// Learning
// ----------------------------------------------
// void SPN::init();
// void SPN::clear_unused_in_SPN();
// void SPN::init_unit_region(Region r);

// ----------------------------------------------
// utils
// ----------------------------------------------
void SPN::print_params()
{
    Utils::println("*** Parameters ***");
    Utils::println("\tdomain = " + Parameter::domain);
    Utils::println("\tnum of Sum Per Region = " + std::to_string(Parameter::num_sum_per_region));
    Utils::println("\tnum of Components Per Variable = " + std::to_string(Parameter::num_components_per_var));
    Utils::println("\tsparse prior = " + std::to_string(Parameter::sparse_prior));
    Utils::println("\tbase resolution = " + std::to_string(Parameter::base_resolution));
    Utils::println("\tnum of Slave Per Class = " + std::to_string(Parameter::num_slave_per_class));
    Utils::println("\tnum of Slave Group = " + std::to_string(Parameter::num_slave_grp));
}