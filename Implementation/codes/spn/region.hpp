#ifndef REGION_HPP
#define REGION_HPP

#include <vector>
#include <map>
#include <string>
#include <unordered_map>  // to use hash map
#include "sum_node.hpp"
#include "prod_node.hpp"
#include "../common/parameter.hpp"
#include "instance.hpp"
#include <memory>

class Region
{
  public:
    int id;
    int a1, a2, b1, b2;
    int a, b; //a = a2 - a1, b = b2 - b1
    int interval; // coarse resolution

    Region() {} // default constructor
    Region(int id, int a1, int a2, int b1, int b2); // todo: test for private
    ~Region() {}

    // pixel region only: gaussian units
    std::vector<double> means;
    std::vector<double> vars;
    std::vector<double> cnts;
    double ttl_cnt;

    // data structure for a parse
    std::unordered_map<int, int> inst_type;
    std::unordered_map<int, std::string> inst_decomp;
    std::unordered_map<std::string, std::shared_ptr<ProdNode> > decomp_prod;

    // each region is alloted a set of sum nodes
    std::vector<std::shared_ptr<SumNode> > types;

    // MAP computation
    int def_map_type_idx;
    std::vector<std::string> map_decomps;
    double def_map_sum_prob;
    double def_map_prod_prob;

    double invar;
    
    /*************************************************************/

    static std::unordered_map<int, std::shared_ptr<Region> > id_regions;
  
    static int get_region_id(int a1, int a2, int b1, int b2);
    static std::shared_ptr<Region> get_region(int id);
    int get_id();
    std::string my_str();
    void reset_types(int num_types);
    void set_types(int num_types);
    void set_base(double val);
    double cmp_Gauss(double v, double mean);
    void set_base_Gauss(double v);
    void set_base_for_sum_out();
    void infer_MAP(int inst_idx, Instance &inst);
    void infer_MAP_for_learning(int inst_idx, Instance &inst);
    void set_cur_parse_to_MAP(int inst_idx);
    void clear_cur_parse(int inst_idx);
    void clear_cur_parse_from_buf(int chosen_type, int ri1, int ri2, int ti1, int ti2);
    void set_cur_parse_from_buf(int chosen_type, int ri1, int ri2, int ti1, int ti2);
};
#endif
