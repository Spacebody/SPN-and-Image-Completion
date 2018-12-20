#ifndef SPN_HPP
#define SPN_HPP

#include <iostream>
// #include "../common/.hpp"
#include <mpi.h>
#include <vector>
#include "instance.hpp"
#include "sum_node.hpp"
#include "region.hpp"
#include "../common/parameter.hpp"
#include <string>

class SPN
{
  public:
    // learning
    static bool is_recording_update;
    std::vector<Instance> training_set;

    // completion
    static bool complete_by_marginal;

    // root
    SumNode root;
    Region root_region;

    // coarser resolution for larger regions
    int coarse_dim_1, coarse_dim_2;

    SPN() : coarse_dim_1(Parameter::input_dim1 / Parameter::base_resolution), \
            coarse_dim_2(Parameter::input_dim2 / Parameter::base_resolution) {}

    // ----------------------------------------------
    // Bottom
    // ----------------------------------------------
    void complete_bottom_img(Instance inst);
    void cmp_MAP_bottom_half(Instance inst);
    void cmp_MAP_bottom_half_marginal(Instance inst);

    // ----------------------------------------------
    // Left
    // ----------------------------------------------
    void complete_left_img(Instance inst);
    void cmp_MAP_left_half(Instance inst);
    void cmp_MAP_left_half_marginal(Instance inst);
    
    // ----------------------------------------------
    // Learning
    // ----------------------------------------------
    void init();
    void clear_unused_in_SPN();

    // ----------------------------------------------
    // Computation
    // ----------------------------------------------
    void cmp_derivative();
    void eval();

    // ----------------------------------------------
    // load/save
    // ----------------------------------------------
    void save_SPN(std::string mdl_name);
    static SPN load_D_SPN();

  private:
    // ----------------------------------------------
    // Bottom
    // ----------------------------------------------
    void infer_MAP_bottom_half(int ii, Instance inst);
    void set_MAP_bottom_to_buf(int inst_idx, Instance inst);

    // ----------------------------------------------
    // Left
    // ----------------------------------------------
    double cmp_marginal(Region r);
    void set_MAP_left_to_buf(int inst_idx, Instance inst);

    // ----------------------------------------------
    // Learning
    // ----------------------------------------------
    void init_unit_region(Region r);

    // ----------------------------------------------
    // Computation
    // ----------------------------------------------
    void cmp_derivative(Region r);
    void eval(Region r);
    void init_derviative(Region r);
    void init_derviative();
    void infer_MAP_left_half(int ii, Instance inst);
    void infer_MAP_for_learning(int ii, Instance inst);
    void clear_cur_parse(int ii);
    void set_cur_parse_to_MAP(int ii);
    void set_cur_parse_from_buf();
    void clear_cur_parse_from_buf();
    void send_update(int dest);
    void recv_update(int src);
    double llh(Instance inst);
    void set_input(Instance inst);
    void set_input_occlude_left_half(Instance inst);
    void set_input_occlude_bottom_half(Instance inst);

    // ----------------------------------------------
    // load/save
    // ----------------------------------------------
    void save_SPN(Region r, std::ostream &out); // TODO: BufferWriter in Java
    static Region load_region(std::vector<std::string> t);
    void add_child(Region r, SumNode n, std::string di, double cc);

    // ----------------------------------------------
    // utils
    // ----------------------------------------------
    void print_params();
};

#endif
