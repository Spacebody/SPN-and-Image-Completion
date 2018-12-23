#include "SPN.hpp"
#include "../common/utils.hpp"
#include "../common/my_mpi.hpp"
#include <set>
#include "decomposition.hpp"

bool SPN::is_recording_update = true;
bool SPN::complete_by_marginal = true;

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
        for (int cb = 0; cb < coarse_dim2; ++cb)
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
                                r.reset_types(Parameter::num_sum_per_region);
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

    r.means = std::vector<double>();
    r.vars = std::vector<double>();
    r.cnts = std::vector<double>();

    int ttl_cnt = this->training_set.size();
    int cnt = int(ceil(ttl_cnt * 1.0 / Parameter::num_components_per_var));

    std::vector<double> vals = std::vector<double>();
    vals.reserve(ttl_cnt);
    for (int ii = 0; ii < training_set.size(); ++ii)
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

// void SPN::eval();

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

// void SPN::eval(Region r);

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

// void SPN::init_derviative();
// void SPN::infer_MAP_left_half(int ii, Instance inst);
// void SPN::infer_MAP_for_learning(int ii, Instance inst);
// void SPN::clear_cur_parse(int ii);
// void SPN::set_cur_parse_to_MAP(int ii);
// void SPN::set_cur_parse_from_buf();
// void SPN::clear_cur_parse_from_buf();
// void SPN::send_update(int dest);
// void SPN::recv_update(int src);
// double SPN::llh(Instance inst);
// void SPN::set_input(Instance inst);
// void SPN::set_input_occlude_left_half(Instance inst);
// void SPN::set_input_occlude_bottom_half(Instance inst);

// ----------------------------------------------
// load/save
// ----------------------------------------------
// void SPN::save_SPN(std::string mdl_name);
// SPN SPN::load_D_SPN();
// void SPN::save_SPN(Region r, std::ostream &out);
// Region SPN::load_region(std::vector<std::string> t);
// void SPN::add_child(Region r, SumNode n, std::string di, double cc);

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