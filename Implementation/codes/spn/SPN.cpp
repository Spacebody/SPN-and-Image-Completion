#include "SPN.hpp"
#include "../common/utils.hpp"
#include "../common/my_mpi.hpp"
#include <set>
#include "decomposition.hpp"
#include <sstream>
#include <algorithm>
#include <iterator>

bool SPN::is_recording_update = true;
bool SPN::complete_by_marginal = true;

// SPN &SPN::operator=(SPN &spn)
// {
//     if (this != &spn)
//     {
//         this->training_set = spn.training_set;
//         this->root = spn.root;
//         this->root_region = spn.root_region;
//         this->coarse_dim1 = spn.coarse_dim1;
//         this->coarse_dim2 = spn.coarse_dim2;
//     }
//     return *this;
// }

// ----------------------------------------------
// Bottom
// ----------------------------------------------
void SPN::complete_bottom_img(Instance inst)
{
    std::string msg = "before comolete bottom half";
    Utils::log_time_ms(msg);
    if (this->complete_by_marginal) 
    {
        this->cmp_MAP_bottom_half_marginal(inst);
        std::string msg2 = "Complete bottom by Marginal";
        Utils::log_time(msg2);
    }
    else
    {
        this->cmp_MAP_bottom_half(inst);
        std::string msg3 = "Complete bottom by MPE";
        Utils::log_time(msg3);
    }
}

void SPN::cmp_MAP_bottom_half(Instance inst)
{
    this->is_recording_update = false; // inference now; no need to update count
    int cmp_idx = -1;  // temp idx for cmp_MAP
    this->infer_MAP_bottom_half(cmp_idx, inst);
    this->set_cur_parse_to_MAP(cmp_idx);
    this->set_MAP_bottom_to_buf(cmp_idx, inst);
    this->clear_cur_parse(cmp_idx);
    this->is_recording_update = true;
}

// compute marginal by differentiation; see Darwiche-03 for details
void SPN::cmp_MAP_bottom_half_marginal(Instance inst)
{
    this->set_input_occlude_bottom_half(inst);
    this->eval();
    this->cmp_derivative();

    for (int i = 0; i < Parameter::input_dim1 / 2; ++i)
    {
        for (int j = 0; j < Parameter::input_dim2; ++j)
            MyMPI::buf_int[MyMPI::buf_idx++] = Utils::get_int_val(inst, inst.vals[i][j]);
    }
    for (int i = Parameter::input_dim1 / 2; i < Parameter::input_dim1; ++i)
    {
        for (int j = 0; j < Parameter::input_dim2; ++j)
        {
            int ri = Region::get_region_id(i, i + 1, j, j + 1);
            Region r = Region::get_region(ri);
            double p = this->cmp_marginal(r);
            MyMPI::buf_int[MyMPI::buf_idx++] = Utils::get_int_val(inst, p);
        }
    }
}

void SPN::infer_MAP_bottom_half(int ii, Instance inst)
{
    this->set_input_occlude_bottom_half(inst);

    // fine region
    for (int ca = 0; ca < this->coarse_dim1; ++ca)
        for (int cb = 0; cb < coarse_dim2; ++cb)
            for (int a = 1; a <= Parameter::base_resolution; ++a)
                for (int b = 1; b <= Parameter::base_resolution; ++b)
                {
                    if (a == 1 && b == 1)
                        continue;
                    for (int a1 = ca * Parameter::base_resolution; a1 <= (ca + 1) * Parameter::base_resolution - a; ++a1)
                    {
                        int a2 = a1 + a;
                        for (int b1 = cb * Parameter::base_resolution; b1 <= (cb + 1) * Parameter::base_resolution - b; ++b1)
                        {
                            int b2 = b1 + b;
                            int ri = Region::get_region_id(a1, a2, b1, b2);
                            Region r = Region::get_region(ri);
                            r.infer_MAP(ii, inst);
                        }
                    }
                }

    // coarse region
    for (int ca = 1; ca <= this->coarse_dim1; ++ca)
        for (int cb = 1; cb <= this->coarse_dim2; ++cb)
        {
            if (ca == 1 && cb == 1)
                continue;

            for (int a1 = 0; a1 <= Parameter::input_dim1 - ca * Parameter::base_resolution; a1 += Parameter::base_resolution)
            {
                int a2 = a1 + ca * Parameter::base_resolution;
                for (int b1 = 0; b1 <= Parameter::input_dim2 - cb * Parameter::base_resolution; b1 += Parameter::base_resolution)
                {
                    int b2 = b1 + cb * Parameter::base_resolution;
                    int ri = Region::get_region_id(a1, a2, b1, b2);
                    Region r = Region::get_region(ri);
                    r.infer_MAP(ii, inst);
                }
            }
        }
}


void SPN::set_MAP_bottom_to_buf(int inst_idx, Instance inst)
{
    for (int a1 = 0; a1 <= Parameter::input_dim1 - 1; ++a1)
    {
        int a2 = a1 + 1;
        for (int b1 = 0; b1 <= Parameter::input_dim2 - 1; ++b1)
        {
            int b2 = b1 + 1;
            if (a1 < Parameter::input_dim2 / 2)
            {
                MyMPI::buf_int[MyMPI::buf_idx++] = Utils::get_int_val(inst, inst.vals[a1][b1]);
            }
            else
            {
                int ri = Region::get_region_id(a1, a2, b1, b2);
                Region r = Region::get_region(ri);
                int vi = r.inst_type[inst_idx];
                MyMPI::buf_int[MyMPI::buf_idx++] = Utils::get_int_val(inst, r.means[vi]);
            }
        }
    }
}

// ----------------------------------------------
// Left
// ----------------------------------------------
void SPN::complete_left_img(Instance inst)
{
    std::string msg = "before comolete left half";
    Utils::log_time_ms(msg);
    if (this->complete_by_marginal)
    {
        this->cmp_MAP_left_half_marginal(inst);
        std::string msg2 = "Complete left by Marginal";
        Utils::log_time(msg2);
    }
    else
    {
        this->cmp_MAP_left_half(inst);
        std::string msg3 = "Complete left by MPE";
        Utils::log_time(msg3);
    }
}

void SPN::cmp_MAP_left_half(Instance inst)
{
    this->is_recording_update = false;
    int cmp_idx = -1;
    this->infer_MAP_left_half(cmp_idx, inst);
    this->set_cur_parse_to_MAP(cmp_idx);
    this->set_MAP_left_to_buf(cmp_idx, inst);
    this->clear_cur_parse(cmp_idx);
    this->is_recording_update = true;
}

// compute marginal by differentiation; see Darwice-03 for details
void SPN::cmp_MAP_left_half_marginal(Instance inst)
{
    this->set_input_occlude_left_half(inst);
    this->eval();
    this->cmp_derivative();

    for (int i = 0; i < Parameter::input_dim1; ++i)
    {
        for (int j = 0; j < Parameter::input_dim2 / 2; ++j)
        {
            int ri = Region::get_region_id(i, i + 1, j, j + 1);
            Region r = Region::get_region(ri);
            double p = this->cmp_marginal(r);
            MyMPI::buf_int[MyMPI::buf_idx++] = Utils::get_int_val(inst, p);
        }
    }
    for (int i = Parameter::input_dim2 / 2; i < Parameter::input_dim2; ++i)
    {
        for (int j = 0; j < Parameter::input_dim2; ++j)
            MyMPI::buf_int[MyMPI::buf_idx++] = Utils::get_int_val(inst, inst.vals[i][j]);
    }
}

double SPN::cmp_marginal(Region r)
{
    double t = 0, d = 0;
    double md = 100;

    for (int i = 0; i < r.types.size(); ++i)
    {
        SumNode n = r.types[i];
        if (n.get_log_derivative() == Node::zero_log_val)
            continue;
        if (md == 100 || n.get_log_derivative() > md)
            md = n.get_log_derivative();
    }
    for (int i = 0; i < r.types.size(); ++i)
    {
        SumNode n = r.types[i];
        if (n.get_log_derivative() == Node::zero_log_val)
            continue;
        double p = exp(n.get_log_derivative() - md);
        d += r.means[i] * p;
        t += p;
    }
    d /= t;
    return d;
}

void SPN::set_MAP_left_to_buf(int inst_idx, Instance inst)
{
    for (int a1 = 0; a1 <= Parameter::input_dim1 - 1; ++a1)
    {
        int a2 = a1 + 1;
        for (int b1 = 0; b1 <= Parameter::input_dim2 - 1; ++b1)
        {
            int b2 = b1 + 1;
            if (a1 >= Parameter::input_dim2 / 2)
            {
                MyMPI::buf_int[MyMPI::buf_idx++] = Utils::get_int_val(inst, inst.vals[a1][b1]);
            }
            else
            {
                int ri = Region::get_region_id(a1, a2, b1, b2);
                Region r = Region::get_region(ri);
                int vi = r.inst_type[inst_idx];
                MyMPI::buf_int[MyMPI::buf_idx++] = Utils::get_int_val(inst, r.means[vi]);
            }
        }
    }
}

// ----------------------------------------------
// Learning
// ----------------------------------------------
void SPN::init()
{
    // coarse region
    for (int ca = 1; ca <= this->coarse_dim1; ++ca)
        for (int cb = 1; cb <= this->coarse_dim2; ++cb)
        {
            if (ca == 1 && cb == 1)
                continue;  // taken care of below in fine
            for (int a1 = 0; a1 <= Parameter::input_dim1 - ca * Parameter::base_resolution; a1 += Parameter::base_resolution)
            {
                int a2 = a1 + ca * Parameter::base_resolution;
                for (int b1 = 0; b1 <= Parameter::input_dim2 - cb * Parameter::base_resolution; b1 += Parameter::base_resolution)
                {
                    int b2 = b1 + cb * Parameter::base_resolution;
                    int ri = Region::get_region_id(a1, a2, b1, b2);
                    Region r = Region::get_region(ri);
                    if (ca == this->coarse_dim1 && this->coarse_dim2)
                    {
                        r.reset_types(1);
                        this->root_region = r;
                        this->root = r.types[0];
                    }
                    else
                        r.reset_types(Parameter::num_sum_per_region);
                }
            }
        }

    // fine region
    for (int ca = 0; ca < this->coarse_dim1; ++ca)
        for (int cb = 0; cb < this->coarse_dim2; ++cb)
            for (int a = 1; a <= Parameter::base_resolution; ++a)
                for (int b = 1; b <= Parameter::base_resolution; ++b)
                {
                    for (int a1 = ca * Parameter::base_resolution; a1 <= (ca + 1) * Parameter::base_resolution - a; ++a1)
                    {
                        int a2 = a1 + a;
                        for (int b1 = cb * Parameter::base_resolution; b1 <= (cb + 1) * Parameter::base_resolution - b; ++b1)
                        {
                            int b2 = b1 + b;
                            int ri = Region::get_region_id(a1, a2, b1, b2);
                            Region r = Region::get_region(ri);
                            if (a == 1 && b == 1)
                            {
                                this->init_unit_region(r);
                            }
                            else
                            {
                                r.reset_types(Parameter::num_sum_per_region); 
                            }
                        }
                    }
                }
}

void SPN::clear_unused_in_SPN()
{
    // coarse
    for (int ca = 1; ca <= this->coarse_dim1; ++ca)
        for (int cb = 1; cb <= this->coarse_dim2; ++cb)
        {
            if (ca == 1 && cb == 1)
                continue; // taken care of below in fine
            for (int a1 = 0; a1 <= Parameter::input_dim1 - ca * Parameter::base_resolution; a1 += Parameter::base_resolution)
            {
                int a2 = a1 + ca * Parameter::base_resolution;
                for (int b1 = 0; b1 <= Parameter::input_dim2 - cb * Parameter::base_resolution; b1 += Parameter::base_resolution)
                {
                    int b2 = b1 + cb * Parameter::base_resolution;
                    int ri = Region::get_region_id(a1, a2, b1, b2);
                    Region r = Region::get_region(ri);
                    std::set<std::string> decomps = std::set<std::string>();
                    for (std::vector<SumNode>::iterator iter = r.types.begin(); iter != r.types.end(); ++iter)
                    {
                        if (iter->get_children().size() > 0)
                        {
                            double tc = 0;
                            for (std::map<std::string, Node>::iterator iter2 = iter->get_children().begin(); iter2 != iter->get_children().end(); ++iter2)
                            {
                                tc += iter->get_child_cnt(iter2->first);
                                decomps.insert(iter2->first);
                            }
                        }
                    }

                    // clear dead decomp_prod
                    std::set<std::string> dead_decomps = std::set<std::string>();
                    for (std::unordered_map<std::string, ProdNode>::iterator iter3 = r.decomp_prod.begin(); iter3 != r.decomp_prod.end(); ++iter3)
                    {
                        if (decomps.count(iter3->first) > 0)
                        {
                            dead_decomps.insert(iter3->first);
                            continue;
                        }
                    }

                    for (std::set<std::string>::iterator iter4 = dead_decomps.begin(); iter4 != dead_decomps.end(); ++iter4)
                    {
                        r.decomp_prod.erase(*iter4);
                        Decomposition::remove(*iter4);
                    }
                }
            }
        }
}

// init: set mean/variance by equal quantiles from training for each pixel
void SPN::init_unit_region(Region r)
{
    r.reset_types(Parameter::num_components_per_var);

    r.means = std::vector<double>(Parameter::num_components_per_var);
    r.vars = std::vector<double>(Parameter::num_components_per_var);
    r.cnts = std::vector<double>(Parameter::num_components_per_var);

    int ttl_cnt = this->training_set.size();
    int cnt = (int)ceil(ttl_cnt * 1.0 / Parameter::num_components_per_var);
    std::vector<double> vals = std::vector<double>(ttl_cnt);
    for (int ii = 0; ii < this->training_set.size(); ++ii)
    {
        vals[ii] = this->training_set[ii].vals[r.a1][r.b1];
    }
    std::sort(vals.begin(), vals.end());
    for (int bi = 0; bi < Parameter::num_components_per_var; ++bi)
    {
        int ac = 0;
        for (int ii = bi * cnt; ii < (bi + 1) * cnt && ii < ttl_cnt; ++ii, ++ac)
        {
            r.means[bi] += vals[ii];
            r.vars[bi] += vals[ii] * vals[ii];
        }
        r.means[bi] /= ac;
        r.vars[bi] /= ac;
        r.vars[bi] -= r.means[bi] * r.means[bi];
        r.cnts[bi] = ac;
    }
    r.ttl_cnts = ttl_cnt;
}

// ----------------------------------------------
// Computation
// ----------------------------------------------
// derivative
void SPN::cmp_derivative()
{
    this->init_derviative();

    this->root.set_log_derivative(0);
    this->root.pass_derivative();
    for (std::map<std::string, Node>::iterator iter = this->root.get_children().begin(); iter != this->root.get_children().end(); ++iter)
    {
        Node n = this->root.get_children()[iter->first];
        n.pass_derivative();
    }

    // coarse region
    for (int ca = this->coarse_dim1; ca >= 1; --ca)
        for (int cb = this->coarse_dim2; cb >= 1; --cb)
        {
            if (ca == 1 && cb == 1)
                continue;

            for (int a1 = 0; a1 <= Parameter::input_dim1 - ca * Parameter::base_resolution; a1 += Parameter::base_resolution)
            {
                int a2 = a1 + ca * Parameter::base_resolution;
                for (int b1 = 0; b1 <= Parameter::input_dim2 - cb * Parameter::base_resolution; b1 += Parameter::base_resolution)
                {
                    int b2 = b1 + cb * Parameter::base_resolution;

                    // coarse regions
                    int ri = Region::get_region_id(a1, a2, b1, b2);
                    Region r = Region::get_region(ri);
                    this->cmp_derivative(r);
                }
            }
        }

    // fine region
    for (int ca = this->coarse_dim1 - 1; ca >= 0; --ca)
        for (int cb = this->coarse_dim2 - 1; cb >= 0; --cb)
            for (int a = Parameter::base_resolution; a >= 1; --a)
                for (int b = Parameter::base_resolution; b >= 1; --b)
                {
                    if (a == 1 && b == 1)
                        continue;
                    for (int a1 = ca * Parameter::base_resolution; a1 <= (ca + 1) * Parameter::base_resolution - a; ++a1)
                    {
                        int a2 = a1 + a;
                        for (int b1 = cb * Parameter::base_resolution; b1 <= (cb + 1) * Parameter::base_resolution - b; ++b1)
                        {
                            int b2 = b1 + b;
                            int ri = Region::get_region_id(a1, a2, b1, b2);
                            Region r = Region::get_region(ri);
                            this->cmp_derivative(r);
                        }
                    }
                }
}

// evaluation:: upward pass
void SPN::eval()
{
    // fine region
    for (int ca = 0; ca < this->coarse_dim1; ++ca)
        for (int cb = 0; cb < this->coarse_dim2; ++cb)
            for (int a = 1; a <= Parameter::base_resolution; ++a)
                for (int b = 1; b <= Parameter::base_resolution; ++b)
                {
                    if (a == 1 && b == 1)
                        continue;  // take care in set_input
                    for (int a1 = ca * Parameter::base_resolution; a1 <= (ca + 1) * Parameter::base_resolution - a; ++a1)
                    {
                        int a2 = a1 + a;
                        for (int b1 = cb * Parameter::base_resolution; b1 <= (cb + 1) * Parameter::base_resolution - b; ++b1)
                        {
                            int b2 = b1 + b;
                            int ri = Region::get_region_id(a1, a2, b1, b2);
                            Region r = Region::get_region(ri);
                            this->eval(r);
                        }
                    }
                }
    
    // coarse region
    for (int ca = 1; ca <= this->coarse_dim1; ++ca)
        for (int cb = 1; cb <= this->coarse_dim2; ++cb)
        {
            if (ca == 1 && cb == 1)
                continue;

            for (int a1 = 0; a1 <= Parameter::input_dim1 - ca * Parameter::base_resolution; a1 += Parameter::base_resolution)
            {
                int a2 = a1 + ca * Parameter::base_resolution;
                for (int b1 = 0; b1 <= Parameter::input_dim2 - cb * Parameter::base_resolution; b1 += Parameter::base_resolution)
                {
                    int b2 = b1 + cb * Parameter::base_resolution;

                    // coarse regions
                    int ri = Region::get_region_id(a1, a2, b1, b2);
                    Region r = Region::get_region(ri);
                    this->eval(r);
                }
            }
        }
}

void SPN::cmp_derivative(Region r)
{
    for (int i = 0; i < r.types.size(); ++i)
    {
        r.types[i].pass_derivative();
    }
    for (std::unordered_map<std::string, ProdNode>::iterator iter = r.decomp_prod.begin(); iter != r.decomp_prod.end(); ++iter)
    {
        Node n = r.decomp_prod[iter->first];
        n.pass_derivative();
    }
}

void SPN::eval(Region r)
{
    for (std::unordered_map<std::string, ProdNode>::iterator iter = r.decomp_prod.begin(); iter != r.decomp_prod.end(); ++iter)
    {
        ProdNode n = r.decomp_prod[iter->first];
        n.eval();
    }
    for (std::vector<SumNode>::iterator iter2 = r.types.begin(); iter2 != r.types.end(); ++iter2)
    {
        if (iter2->get_children().size() > 0)
            iter2->eval();
        else
            iter2->set_log_val(Node::zero_log_val);
    }
}

void SPN::init_derviative(Region r)
{
    for (std::unordered_map<std::string, ProdNode>::iterator iter = r.decomp_prod.begin(); iter != r.decomp_prod.end(); ++iter)
    {
        ProdNode n = r.decomp_prod[iter->first];
        n.set_log_derivative(Node::zero_log_val);
    }
    for (std::vector<SumNode>::iterator iter2 = r.types.begin(); iter2 != r.types.end(); ++iter2)
    {
        iter2->set_log_derivative(Node::zero_log_val);
    }
}

void SPN::init_derviative()
{
    for (int ca = this->coarse_dim1; ca >= 1; --ca)
        for (int cb = this->coarse_dim2; cb >= 1; --cb)
        {
            if (ca ==1 && cb ==1)
                continue;

            for (int a1 = 0; a1 <= Parameter::input_dim1 - ca * Parameter::base_resolution; a1 += Parameter::base_resolution)
            {
                int a2 = a1 + ca * Parameter::base_resolution;
                for (int b1 = 0; b1 <= Parameter::input_dim2 - ca * Parameter::base_resolution; b1 += Parameter::base_resolution)
                {
                    int b2 = b1 + cb * Parameter::base_resolution;

                    // coarse regions
                    int ri = Region::get_region_id(a1, a2, b1, b2);
                    Region r = Region::get_region(ri);
                    this->init_derviative(r);
                }
            }
        }

    // fine region
    for (int ca = this->coarse_dim1 - 1; ca >= 0; --ca)
        for (int cb = this->coarse_dim2 - 1; cb >= 0; --cb)
            for (int a = Parameter::base_resolution; a >= 1; --a)
                for (int b = Parameter::base_resolution; b >= 1; --b)
                {
                    for (int a1 = ca * Parameter::base_resolution; a1 <= (ca + 1) * Parameter::base_resolution - a; ++a1)
                   {
                       int a2 = a1 + a;
                       for (int b1 = cb * Parameter::base_resolution; b1 <= (cb + 1) * Parameter::base_resolution - b; ++b1)
                       {
                           int b2 = b1 + b;
                           int ri = Region::get_region_id(a1, a2, b1, b2);
                           Region r = Region::get_region(ri);
                           this->init_derviative(r);
                       }
                    }
                }
}

// compute MAP
void SPN::infer_MAP_left_half(int ii, Instance inst)
{
    this->set_input_occlude_left_half(inst);

    // fine region
    for (int ca = 0; ca < this->coarse_dim1; ++ca)
        for (int cb = 1; cb <= this->coarse_dim2; ++cb)
            for (int a = 1; a <= Parameter::base_resolution; ++a)
                for (int b = 1; b <= Parameter::base_resolution; ++b)
                {
                    if (ca == 1 && cb == 1)
                        continue;

                    for (int a1 = ca * Parameter::base_resolution; a1 <= (ca + 1) * Parameter::base_resolution - a; ++a1)
                    {
                        int a2 = a1 + a;
                        for (int b1 = cb * Parameter::base_resolution; b1 <= (cb + 1) * Parameter::base_resolution - b; ++b1)
                        {
                            int b2 = b1 + cb * Parameter::base_resolution;
                            int ri = Region::get_region_id(a1, a2, b1, b2);
                            Region r = Region::get_region(ri);
                            r.infer_MAP(ii, inst);
                        }
                    }
                }

    // coarse region
    for (int ca = 1; ca <= this->coarse_dim1; ++ca)
        for (int cb = 1; cb <= this->coarse_dim2; ++cb)
        {
            if (ca == 1 && cb == 1)
                continue;  // taken care of below in fine

            for (int a1 = 0; a1 <= Parameter::input_dim1 - ca * Parameter::base_resolution; a1 += Parameter::base_resolution)
            {
                int a2 = a1 + ca * Parameter::base_resolution;
                for (int b1 = 0; b1 <= Parameter::input_dim2 - cb * Parameter::base_resolution; b1 += Parameter::base_resolution)
                {
                    int b2 = b1 + cb * Parameter::base_resolution;
                    int ri = Region::get_region_id(a1, a2, b1, b2);
                    Region r = Region::get_region(ri);
                    r.infer_MAP(ii, inst);
                }
            }
        }
}

void SPN::infer_MAP_for_learning(int ii, Instance inst)
{
    this->set_input(inst);

    //fine region
    for (int ca = 0; ca < this->coarse_dim1; ++ca)
        for (int cb = 1; cb <= this->coarse_dim2; ++cb)
            for (int a = 1; a <= Parameter::base_resolution; ++a)
                for (int b = 1; b <= Parameter::base_resolution; ++b)
                {
                    if (ca == 1 && cb == 1)
                        continue;
                    for (int a1 = ca * Parameter::base_resolution; a1 <= (ca + 1) * Parameter::base_resolution - a; ++a1)
                    {
                        int a2 = a1 + a;
                        for (int b1 = cb * Parameter::base_resolution; b1 <= (cb + 1) * Parameter::base_resolution - b; ++b1)
                        {
                            int b2 = b1 + cb * Parameter::base_resolution;
                            int ri = Region::get_region_id(a1, a2, b1, b2);
                            Region r = Region::get_region(ri);
                            r.infer_MAP_for_learning(ii, inst);
                        }
                    }
                }

    // coarse region
    for (int ca = 1; ca <= this->coarse_dim1; ++ca)
        for (int cb = 1; cb <= this->coarse_dim2; ++cb)
        {
            if (ca == 1 && cb == 1)
                continue; // taken care of below in fine
            for (int a1 = 0; a1 <= Parameter::input_dim1 - ca * Parameter::base_resolution; a1 += Parameter::base_resolution)
            {
                int a2 = a1 + ca * Parameter::base_resolution;
                for (int b1 = 0; b1 <= Parameter::input_dim2 - cb * Parameter::base_resolution; b1 += Parameter::base_resolution)
                {
                    int b2 = b1 + cb * Parameter::base_resolution;
                    int ri = Region::get_region_id(a1, a2, b1, b2);
                    Region r = Region::get_region(ri);
                    r.infer_MAP_for_learning(ii, inst);
                }
            }
        }
}

// clear/set parse
void SPN::clear_cur_parse(int ii)
{
    this->root_region.clear_cur_parse(ii);
}

void SPN::set_cur_parse_to_MAP(int ii)
{
    this->root_region.set_cur_parse_to_MAP(ii);
}

void SPN::set_cur_parse_from_buf()
{
    // --- update format: inst_id, region_id, type, decomp(rid1, rid2, type1, type2) - in buf_inf
    int k = 0;
    while ( k < MyMPI::buf_idx)
    {
        int ri = MyMPI::buf_int[k++];
        int chosen_type = MyMPI::buf_int[k++];
        int ri1 = MyMPI::buf_int[k++];
        int ri2 = MyMPI::buf_int[k++];
        int ti1 = MyMPI::buf_int[k++];
        int ti2 = MyMPI::buf_int[k++];
        Region r = Region::get_region(ri);
        r.set_cur_parse_from_buf(chosen_type, ri1, ri2, ti1, ti2);
    }
}

void SPN::clear_cur_parse_from_buf()
{
    // --- update format: inst_id, region_id, type, decomp(rid1, rid2, type1, type2) - in buf_inf
    int k = 0;
    while (k < MyMPI::buf_idx)
    {
        int ri = MyMPI::buf_int[k++];
        int chosen_type = MyMPI::buf_int[k++];
        int ri1 = MyMPI::buf_int[k++];
        int ri2 = MyMPI::buf_int[k++];
        int ti1 = MyMPI::buf_int[k++];
        int ti2 = MyMPI::buf_int[k++];
        Region r = Region::get_region(ri);
        r.clear_cur_parse_from_buf(chosen_type, ri1, ri2, ti1, ti2);
    }
}

void SPN::send_update(int dest)
{
    if (MyMPI::buf_idx >= MyMPI::buf_size)
        Utils::println("ERR: buffer overflow to" + std::to_string(dest));
    // MPI::COMM_WORLD.Send(&MyMPI::buf_int, MyMPI::buf_idx, MPI::INT, dest, 0);
    MPI_Send(&MyMPI::buf_int, MyMPI::buf_idx, MPI_INT, dest, 0, MPI_COMM_WORLD);
}

void SPN::recv_update(int src)
{
    // MPI::Status status;
    MPI_Status status;
    // MPI::COMM_WORLD.Recv(&MyMPI::buf_int, MyMPI::buf_idx, MPI::INT, src, 0, status);
    MPI_Recv(&MyMPI::buf_int, MyMPI::buf_idx, MPI_INT, src, 0, MPI_COMM_WORLD, &status);
    // MyMPI::buf_idx += status.Get_count(MPI::INT);
    int count;
    MPI_Get_count(&status, MPI_INT, &count);
    MyMPI::buf_idx += count;
    if (MyMPI::buf_idx >= MyMPI::buf_size)
        Utils::println("ERR: buffer overflow from" + std::to_string(src));
}

// compute log probability
double SPN::llh(Instance inst)
{
    this->set_input(inst);
    this->eval();
    return this->root.get_log_val();
}

// set dspn input
void SPN::set_input(Instance inst)
{
    for (int a1 = 0; a1 <= Parameter::input_dim1 - 1; ++a1)
    {
        int a2 = a1 + 1;
        for (int b1 = 0; b1 <= Parameter::input_dim2 - 1; ++b1)
        {
            int b2 = b1 + 1;
            int ri = Region::get_region_id(a1, a2, b1, b2);
            Region r = Region::get_region(ri);
            r.set_base(inst.vals[a1][b1]);
        }
    }
}

void SPN::set_input_occlude_left_half(Instance inst)
{
    for (int a1 = 0; a1 <= Parameter::input_dim1 - 1; ++a1)
    {
        int a2 = a1 + 1;
        for (int b1 = 0; b1 <= Parameter::input_dim2 - 1; ++b1)
        {
            int b2 = b1 + 1;
            int ri = Region::get_region_id(a1, a2, b1, b2);
            Region r = Region::get_region(ri);
            if (b1 < Parameter::input_dim2 / 2) // r.set_base(0, 0); // log 1, 1
                r.set_base_for_sum_out();
            else
                r.set_base(inst.vals[a1][b1]);
        }
    }
}

void SPN::set_input_occlude_bottom_half(Instance inst)
{
    for (int a1 = 0; a1 <= Parameter::input_dim1 - 1; ++a1)
    {
        int a2 = a1 + 1;
        for (int b1 = 0; b1 <= Parameter::input_dim2 - 1; ++b1)
        {
            int b2 = b1 + 1;
            int ri = Region::get_region_id(a1, a2, b1, b2);
            Region r = Region::get_region(ri);
            if (b1 < Parameter::input_dim2 / 2)
                r.set_base_for_sum_out();
            else
                r.set_base(inst.vals[a1][b1]);
        }
    }
}

// ----------------------------------------------
// load/save
// ----------------------------------------------
void SPN::save_DSPN(std::string mdl_name)
{
    std::string file_name = mdl_name + ".mdl";
    std::fstream out(file_name, std::fstream::out);

    // fine region
    for (int ca = 0; ca < this->coarse_dim1; ++ca)
        for (int cb = 1; cb <= this->coarse_dim2; ++cb)
            for (int a = 1; a <= Parameter::base_resolution; ++a)
                for (int b = 1; b <= Parameter::base_resolution; ++b)
                {
                    // if (ca == 1 && cb == 1)
                        // continue;
                    for (int a1 = ca * Parameter::base_resolution; a1 <= (ca + 1) * Parameter::base_resolution - a; ++a1)
                    {
                        int a2 = a1 + a;
                        for (int b1 = cb * Parameter::base_resolution; b1 <= (cb + 1) * Parameter::base_resolution - b; ++b1)
                        {
                            int b2 = b1 + cb * Parameter::base_resolution;
                            int ri = Region::get_region_id(a1, a2, b1, b2);
                            Region r = Region::get_region(ri);
                            this->save_region(r, out);
                        }
                    }
                }

    // coarse region
    for (int ca = 1; ca <= this->coarse_dim1; ++ca)
        for (int cb = 1; cb <= this->coarse_dim2; ++cb)
        {
            if (ca == 1 && cb == 1)
                continue; // taken care of below in fine
            for (int a1 = 0; a1 <= Parameter::input_dim1 - ca * Parameter::base_resolution; a1 += Parameter::base_resolution)
            {
                int a2 = a1 + ca * Parameter::base_resolution;
                for (int b1 = 0; b1 <= Parameter::input_dim2 - cb * Parameter::base_resolution; b1 += Parameter::base_resolution)
                {
                    int b2 = b1 + cb * Parameter::base_resolution;
                    int ri = Region::get_region_id(a1, a2, b1, b2);
                    Region r = Region::get_region(ri);
                    this->save_region(r, out);
                }
            }
        }
    out.close();
}

void SPN::save_region(Region r, std::fstream &out)
{
    std::string s;

    // region ID
    out << "<REGION>\n";
    out << std::to_string(r.a1) + " " + std::to_string(r.a2) + " " + std::to_string(r.b1) + " " + std::to_string(r.b2) + "\n";

    // type -> decomp / cnt
    out << "<TYPE>\n";
    out << std::to_string(r.types.size()) + "\n";
    for (int i = 0; i < r.types.size(); ++i)
    {
        SumNode n = r.types[i];
        s = std::to_string(n.get_cnt()) + "";
        for (std::map<std::string, Node>::iterator iter = n.get_children().begin(); iter != n.get_children().end(); ++iter)
        {
            s += ":<" + iter->first + ">:" + std::to_string(n.get_child_cnt(iter->first));
        }
        out << s + "\n";
    }
    out << "</TYPE>\n";

    // unit:
    if (r.a == 1 && r.b == 1)
    {
        out << "<MEAN>" + std::to_string(r.a1) + " " + std::to_string(r.b1) + ":";
        for (int i = 0; i < r.means.size(); ++i)
            out << " " + std::to_string(r.means[i]);
        out << "\n";
        out << "<CNT> " + std::to_string(r.a1) + " " + std::to_string(r.b1) + ":";
        for (int i = 0; i < r.cnts.size(); ++i)
            out << " " + std::to_string(r.cnts[i]);
        out << "\n";
    }
    out << "</REGION>\n";
}

SPN SPN::load_DSPN(std::string mdl_name)
{
    std::string file_name = mdl_name + ".mdl";
    std::fstream in(file_name, std::fstream::in);

    SPN dspn = SPN();

    std::string s;
    std::vector<std::string> t;
    while (std::getline(in, s))
    {
        if (s.find("<REGION>") == std::string::npos)
            t = std::vector<std::string>();
        else if (s == "</REGION>")
        {
            Region r = SPN::load_region(t);
            if (r.types.size() == 1)
            {
                dspn.root_region = r;
                dspn.root = r.types[0];
                t.clear();
            }
        }
        else
        {
            if (t.empty())
                Utils::println("ERR: " + s);
            t.push_back(s);
        }
    }
    in.close();
    return dspn;
}

Region SPN::load_region(std::vector<std::string> t)
{
    int a1, a2, b1, b2;
    std::string s;
    std::vector<std::string> ts;
    int idx = 0;
    s = t[idx++];
    std::stringstream ss(s);  // split by ' '
    ts = std::vector<std::string>((std::istream_iterator<std::string>(ss)), std::istream_iterator<std::string>());
    a1 = std::stoi(ts[0]);
    a2 = std::stoi(ts[1]);
    b1 = std::stoi(ts[2]);
    b2 = std::stoi(ts[3]);
    Region r = Region::get_region(Region::get_region_id(a1, a2, b1, b2));

    // type
    s = t[idx++];  // <TYPE>
    s = t[idx++];
    int num_types = std::stoi(s);
    r.reset_types(num_types);
    for (int i = 0; i < num_types; ++i)
    {
        s = t[idx++];
        std::stringstream ss(s);
        ts.clear();
        while (std::getline(ss, s, ':'))
            ts.push_back(s); // split by ':'
        SumNode n = r.types[i];
        n.set_cnt(std::stod(ts[0]));
        for (int j = 1; j < ts.size(); j += 2)
        {
            std::string di = ts[j];
            di = di.substr(1, di.length() - 1);
            double cc = std::stod(ts[j + 1]);
            SPN::add_child(r, n, di, cc);
        }
    }
    s = t[idx++]; // </TYPE>

    // unit
    if (idx < t.size())
    {
        s = t[idx++];
        if (s.find("<MEAN>") != std::string::npos)
        {
            Utils::println("ERR: not mean: " + s);
            exit(-1);
        }
        s = s.substr(s.find(":") + 1);
        ts.clear();
        std::stringstream ss(s); // split by ' '
        ts = std::vector<std::string>((std::istream_iterator<std::string>(ss)), std::istream_iterator<std::string>());
        r.means = std::vector<double>();
        for (int i = 0; i < ts.size(); ++i)
            r.means.push_back(std::stod(ts[i]));
        s = t[idx++];
        if (s.find("<CNT>") != std::string::npos)
        {
            Utils::println("ERR: not cnt: " + s);
            exit(-1);
        }
        s = s.substr(s.find(":") + 1);
        ts.clear();
        std::stringstream ss2(s); // split by ' '
        ts = std::vector<std::string>((std::istream_iterator<std::string>(ss2)), std::istream_iterator<std::string>());
        r.cnts = std::vector<double>();
        for (int i = 0; i < ts.size(); ++i)
            r.cnts.push_back(std::stod(ts[i]));
    }
    
    return r;
}

void SPN::add_child(Region r, SumNode n, std::string di, double cc)
{
    n.set_child_cnt(di, cc);
    ProdNode np;
    if (r.decomp_prod.count(di) > 0)
    {
        Decomposition d = Decomposition::get_decomposition(di);
        np = ProdNode();
        Region r1 = Region::get_region(d.type_id_1);
        Region r2 = Region::get_region(d.type_id_2);
        np.add_child(r1.types[d.type_id_1]);
        np.add_child(r2.types[d.type_id_2]);
        r.decomp_prod.insert(std::pair<std::string, ProdNode>(di, np));
    }
    n.add_child_only(di, np);
}

// ----------------------------------------------
// utils
// ----------------------------------------------
void SPN::print_params()
{
    Utils::println("*** Parameters ***");
    Utils::println("\tDomain = " + Parameter::domain);
    Utils::println("\tNum of Sum Per Region = " + std::to_string(Parameter::num_sum_per_region));
    Utils::println("\tNum of Components Per Variable = " + std::to_string(Parameter::num_components_per_var));
    Utils::println("\tSparse Prior = " + std::to_string(Parameter::sparse_prior));
    Utils::println("\tBase Resolution = " + std::to_string(Parameter::base_resolution));
    Utils::println("\tNum of Slave Per Class = " + std::to_string(Parameter::num_slave_per_class));
    Utils::println("\tNum of Slave Group = " + std::to_string(Parameter::num_slave_grp));
}
