#include "region.hpp"
#include <cmath>
#include "../common/utils.hpp"

std::unordered_map<int, Region> Region::id_regions = std::unordered_map<int, Region>();

Region::Region(int id, int a1, int a2, int b1, int b2)
{
    this->id = id;
    this->a1 = a1;
    this->a2 = a2;
    this->b1 = b1;
    this->b2 = b2;
    this->a = a2 - a1;
    this->b = b2 - b1;
    
    if(this->a > Parameter::base_resolution || this->b > Parameter::base_resolution)
    {
        if(this->a % Parameter::base_resolution != 0 || this->b % Parameter::base_resolution != 0)
        {
            Utils::println("ERR: base_res=" + std::to_string(Parameter::base_resolution) + " " + \
                    std::to_string(a1) + ", " + std::to_string(a2) + ", " + std::to_string(b1) + ", " + std::to_string(b2));
            exit(-1);
        }
        if(this->a <= Parameter::base_resolution && this->b <= Parameter::base_resolution)
            this->interval = 1;
        else
            this->interval = Parameter::base_resolution;
    }

    this->ttl_cnts = 0;
    this->inst_type = std::unordered_map<int, int>();
    this->inst_decomp = std::unordered_map<int, std::string>();
    this->decomp_prod = std::unordered_map<std::string, ProdNode>();

    this->types = std::vector<SumNode>();

    this->map_decomps = std::vector<std::string>();

    this->invar = sqrt(20);
}

int Region::get_region_id(int a1, int a2, int b1, int b2)
{
    int id = ((a1 * Parameter::input_dim1 + a2 - 1) * Parameter::input_dim2 + b1) * Parameter::input_dim2 + b2 - 1;
    if (Region::id_regions.count(id) > 0)
        Region::id_regions.insert(std::pair<int, Region>(id, Region(id, a1, a2, b1, b2)));  // TODO: DEBUG
    return id;
}

Region Region::get_region(int id)
{
    Region r;
    if (!id_regions.count(id))
    {
        int b2 = id % Parameter::input_dim2 + 1;
        int x = id / Parameter::input_dim2;
        int b1 = x % Parameter::input_dim2;
        x = x / Parameter::input_dim2;
        int a2 = x % Parameter::input_dim1 + 1;
        int a1 = x / Parameter::input_dim1;
        r = Region::get_region(Region::get_region_id(a1, a2, b1, b2));
    }
    else
        r = Region::id_regions[id];
    return r;
}

int Region::get_id() 
{ 
    return this->id; 
}

std::string Region::my_str()
{
    std::string s = "<" + std::to_string(this->a1) + ", " + std::to_string(this->a2) + ", " + std::to_string(this->b1) + ", " + std::to_string(b2) + ">";
    return s;
}

void Region::reset_types(int num_types)
{
    this->types.clear();
    this->inst_type.clear();
    this->inst_decomp.clear();
    this->decomp_prod.clear();
    this->map_decomps.clear(); // as NULL
    for (int i = 0; i < num_types; ++i)
    {
        this->types.push_back(SumNode());
    }
}

void Region::set_types(int num_types)
{
    if (num_types < this->types.size())
    {
        this->reset_types(num_types);
        return;
    }
    int nn = num_types - this->types.size();
    for (int i = 0; i < nn; ++i)
    {
        this->types.push_back(SumNode());
    }
}

int Region::set_base(double val)
{
    this->set_Gauss_base(val);
}

double Region::cmp_Gauss(double v, double mean)
{
    double m = mean - v;
    return -(m * m / 2);
}

void Region::set_Gauss_base(double v)
{
    this->def_map_type_idx = -1;
    double mp = 0;
    for (int i = 0; i < this->types.size(); ++i)
    {
        SumNode n = this->types[i];
        n.set_log_val(this->cmp_Gauss(v, this->means[i]));
        if (this->def_map_type_idx == -1 || n.get_log_val() > mp)
        {
            this->def_map_type_idx = i;
            mp = n.get_log_val();
        }
    }
}

void Region::set_Gauss_for_sum_out()
{
    this->def_map_type_idx = -1;
    for (int i = 0; i < Parameter::num_components_per_var; ++i)
    {
        SumNode n = this->types[i];
        n.set_log_val(0);
    }
}

// void Region::infer_MAP(int inst_idx, Instance inst);
// void Region::infer_MAP_for_learning(int inst_idx, Instance inst);
// void Region::set_cur_parse_to_MAP(int inst_idx);
// void Region::clear_cur_parse(int inst_idx);
// void Region::clear_cur_parse_from_buf(int chosen_type, int ri1, int ri2, int ti1, int ti2);
// void Region::set_cur_parse_from_buf(int chosen_type, int ri1, int ri2, int ti1, int ti2);
