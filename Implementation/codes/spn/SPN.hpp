#ifndef SPN_HPP
#define SPN_HPP

#include <iostream>
#include <vector>
#include "instance.hpp"
#include "sum_node.hpp"
#include "region.hpp"
#include "../common/parameter.hpp"
#include <string>
#include <fstream>

class SPN
{
  public:
    // learning
    static bool is_recording_update;  // record update in clear_parse/set_cur_parse
    std::vector<Instance> training_set;

    // completion
    static bool complete_by_marginal;  // complete pixel by marginal

    // root
    SumNode root;
    Region root_region;

    // coarser resolution for larger regions
    int coarse_dim1, coarse_dim2;

    SPN() : coarse_dim1(Parameter::input_dim1 / Parameter::base_resolution), \
            coarse_dim2(Parameter::input_dim2 / Parameter::base_resolution) {}
    ~SPN() {}

    // ----------------------------------------------
    // Bottom
    // ----------------------------------------------
    void complete_bottom_img(Instance &inst);
    void cmp_MAP_bottom_half(Instance &inst);
    void cmp_MAP_bottom_half_marginal(Instance &inst);

    // ----------------------------------------------
    // Left
    // ----------------------------------------------
    void complete_left_img(Instance &inst);
    void cmp_MAP_left_half(Instance &inst);
    void cmp_MAP_left_half_marginal(Instance &inst);
    
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
    void send_update(int dest);
    void recv_update(int src);
    void clear_cur_parse(int ii);
    void set_cur_parse_from_buf();
    void clear_cur_parse_from_buf();
    void infer_MAP_for_learning(int ii, Instance &inst);
    void set_cur_parse_to_MAP(int ii);
    double llh(Instance &inst);

    // ----------------------------------------------
    // load/save
    // ----------------------------------------------
    void save_DSPN(std::string mdl_name);
    static SPN load_DSPN(std::string mdl_name);

    // ----------------------------------------------
    // utils
    // ----------------------------------------------
    void print_params();

  private:
    // ----------------------------------------------
    // Bottom
    // ----------------------------------------------
    void infer_MAP_bottom_half(int ii, Instance &inst);
    void set_MAP_bottom_to_buf(int inst_idx, Instance &inst);

    // ----------------------------------------------
    // Left
    // ----------------------------------------------
    double cmp_marginal(Region &r);
    void set_MAP_left_to_buf(int inst_idx, Instance &inst);

    // ----------------------------------------------
    // Learning
    // ----------------------------------------------
    void init_unit_region(Region &r);

    // ----------------------------------------------
    // Computation
    // ----------------------------------------------
    void cmp_derivative(Region &r);
    void eval(Region &r);
    void init_derviative(Region &r);
    void init_derviative();
    void infer_MAP_left_half(int ii, Instance &inst);
    void set_input(Instance &inst);
    void set_input_occlude_left_half(Instance &inst);
    void set_input_occlude_bottom_half(Instance &inst);

    // ----------------------------------------------
    // load/save
    // ----------------------------------------------
    void save_region(Region &r, std::fstream &fs);
    static Region load_region(std::vector<std::string> t);
    static void add_child(Region &r, SumNode &n, std::string di, double cc);

};

#endif
