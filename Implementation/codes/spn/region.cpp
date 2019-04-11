#include "region.hpp"
#include <cmath>
#include "../common/utils.hpp"
#include "decomposition.hpp"
#include "../common/my_mpi.hpp"
#include "SPN.hpp"

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
    }

    if (this->a <= Parameter::base_resolution && this->b <= Parameter::base_resolution)
        this->interval = 1;
    else
        this->interval = Parameter::base_resolution;

    this->ttl_cnt = 0;

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
    if (Region::id_regions.count(id) == 0)
        Region::id_regions.insert(std::pair<int, Region>(id, Region(id, a1, a2, b1, b2)));  // mark
    return id;
}

Region &Region::get_region(int id)
{
    if (id_regions.count(id) == 0)
    {
        int b2 = id % Parameter::input_dim2 + 1;
        int x = id / Parameter::input_dim2;
        int b1 = x % Parameter::input_dim2;
        x = x / Parameter::input_dim2;
        int a2 = x % Parameter::input_dim1 + 1;
        int a1 = x / Parameter::input_dim1;
        return Region::get_region(Region::get_region_id(a1, a2, b1, b2));
    }
    else
        return Region::id_regions[id];
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

// initialization
void Region::reset_types(int num_types)
{   // clean up
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

// set value for input layer
void Region::set_base(double val)
{
    this->set_base_Gauss(val);
}

double Region::cmp_Gauss(double v, double mean)
{
    double m = mean - v;
    return -(m * m / 2);
}

void Region::set_base_Gauss(double v)
{
    this->def_map_type_idx = -1;
    double mp = 0;
    for (int i = 0; i < this->types.size(); ++i)
    {
        SumNode &n = this->types[i];
        n.log_val = this->cmp_Gauss(v, this->means[i]);
        if (this->def_map_type_idx == -1 || n.log_val > mp)
        {
            this->def_map_type_idx = i;
            mp = n.log_val;
        }
    }
}

void Region::set_base_for_sum_out()
{
    this->def_map_type_idx = -1;
    for (int i = 0; i < Parameter::num_components_per_var; ++i)
    {
        SumNode &n = this->types[i];
        n.log_val = 0;
    }
}

// compute MAP state at inference time
void Region::infer_MAP(int inst_idx, Instance &inst)
{
    if (this->map_decomps.empty())
        this->map_decomps.resize(this->types.size());
    
    // compute prod values
    for (std::unordered_map<std::string, ProdNode>::iterator iter = this->decomp_prod.begin();
         iter != this->decomp_prod.end(); ++iter)
    {
        ProdNode &n = this->decomp_prod[iter->first];
        n.eval();
    }

    // evaluate children for sum nodes
    for (int ti = 0; ti < this->types.size(); ++ti)
    {
        if (this->types[ti].children.size() == 0)
            continue;

        SumNode &n = this->types[ti];
        n.eval();

        double max_child_prob = 0;
        std::vector<std::string> map_decomp_opt = std::vector<std::string>();
        for (std::map<std::string, Node>::iterator iter2 = n.children.begin();
             iter2 != n.children.end(); ++iter2)
        {
            Node &c = n.children[iter2->first];
            double m = (c.log_val == Node::zero_log_val) ? Node::zero_log_val : c.log_val + log(n.get_child_cnt(iter2->first));

            if (map_decomp_opt.empty() || m > max_child_prob)
            {
                map_decomp_opt.clear();
                max_child_prob = m;
            }
            if (m == max_child_prob)
            {
                map_decomp_opt.push_back(iter2->first);
            }
        }

        // randomly break tie
        this->map_decomps[ti] = map_decomp_opt[Utils::random_next_int(map_decomp_opt.size())];
        map_decomp_opt.clear();
    }
}

// compute MAP state at learning time: could tap a previous unused node
void Region::infer_MAP_for_learning(int inst_idx, Instance &inst)
{
    std::vector<std::string> def_map_decomp_opts = std::vector<std::string>();
    std::string def_map_decomp = "";
    if (this->map_decomps.empty())
        this->map_decomps.resize(this->types.size());

    this->def_map_type_idx = -1;
    this->def_map_sum_prob = 100;
    this->def_map_prod_prob = 100;
    def_map_decomp_opts.clear();

    // sum: choose a previous unused node
    std::vector<int> blanks = std::vector<int>();
    for (int i = 0; i < this->types.size(); ++i)
    {
        SumNode &n = this->types[i];
        if (n.children.size() == 0)
        {
            // if (blanks.empty())
                // blanks = std::vector<int>();
            blanks.push_back(i);
        }
    }
    
    int chosen_blank_idx = -1;

    if (!blanks.empty())
    {
        if (blanks.size() > 1)
        {
            int ci = Utils::random_next_int(blanks.size());
            chosen_blank_idx = blanks[ci];
        }
        else
            chosen_blank_idx = blanks[0];
        blanks.clear();
    }

    // find MAP decomposition
    for (int i = this->a1 + this->interval; i < this->a2; i += this->interval)
    {
        int ri1 = Region::get_region_id(this->a1, i, this->b1, this->b2);
        int ri2 = Region::get_region_id(i, this->a2, this->b1, this->b2);
        Region &r1 = Region::get_region(ri1);
        Region &r2 = Region::get_region(ri2);

        SumNode &n1 = r1.types[r1.def_map_type_idx];
        SumNode &n2 = r2.types[r2.def_map_type_idx];
        double lp;

        if (n1.log_val == Node::zero_log_val || n2.log_val == Node::zero_log_val)
            lp = Node::zero_log_val;
        else
            lp = n1.log_val + n2.log_val;

        if (def_map_decomp_opts.empty() || lp > this->def_map_prod_prob)
        {
            this->def_map_prod_prob = lp;
            def_map_decomp_opts.clear();
        }
        if (lp == this->def_map_prod_prob)
        {
            std::string di = Decomposition::get_id_str(ri1, ri2, r1.def_map_type_idx, r2.def_map_type_idx);
            def_map_decomp_opts.push_back(di);
        }
    }
    for (int i = this->b1 + this->interval; i < this->b2; i += this->interval)
    {
        int ri1 = Region::get_region_id(this->a1, this->a2, this->b1, i);
        int ri2 = Region::get_region_id(this->a1, this->a2, i, this->b2);
        Region &r1 = Region::get_region(ri1);
        Region &r2 = Region::get_region(ri2);

        SumNode &n1 = r1.types[r1.def_map_type_idx];
        SumNode &n2 = r2.types[r2.def_map_type_idx];
        double lp;

        if (n1.log_val == Node::zero_log_val || n2.log_val == Node::zero_log_val)
            lp = Node::zero_log_val;
        else
            lp = n1.log_val + n2.log_val;

        if (def_map_decomp_opts.empty() || lp > this->def_map_prod_prob)
        {
            this->def_map_prod_prob = lp;
            def_map_decomp_opts.clear();
        }

        if (lp == this->def_map_prod_prob)
        {
            std::string di = Decomposition::get_id_str(ri1, ri2, r1.def_map_type_idx, r2.def_map_type_idx);
            def_map_decomp_opts.push_back(di);
        }
    }

    // random break ties for a previously unused node
    def_map_decomp = def_map_decomp_opts[Utils::random_next_int(def_map_decomp_opts.size())];
    def_map_decomp_opts.clear();

    // evaluate product nodes
    for (std::unordered_map<std::string, ProdNode>::iterator iter = this->decomp_prod.begin();
            iter != this->decomp_prod.end(); ++iter)
    {
        ProdNode &n = this->decomp_prod[iter->first];
        n.eval();
    }

    // evaluate existing sum nodes and children
    std::vector<int> map_types = std::vector<int>();
    for (int ti = 0; ti < this->types.size(); ++ti)
    {
        if (this->types[ti].children.size() == 0)
            continue;
        SumNode &n = this->types[ti];
        n.eval();

        double max_sum_prob = 0;
        std::vector<std::string> map_decomp_opt = std::vector<std::string>();

        for (std::map<std::string, Node>::iterator iter = n.children.begin();
                iter != n.children.end(); ++iter)
        {
            Node &c = n.children[iter->first];
            double l = n.log_val + log(n.cnt);
            double m = c.log_val;
            double nl;

            if (l > m) 
            {
                nl = l + log(1 + exp(m - l));
            }
            else
            {
                nl = m + log(1 + exp(l - m));
            }

            if (map_decomp_opt.empty() || nl > max_sum_prob)
            {
                map_decomp_opt.clear();
                max_sum_prob = nl;
            }
            if (nl == max_sum_prob)
            {
                map_decomp_opt.push_back(iter->first);
            }
        }

        if (n.children.count(def_map_decomp) == 0)
        {
            double nl = this->def_map_prod_prob;
            if (n.log_val != Node::zero_log_val)
            {
                nl = log(n.cnt) + n.log_val;
                if (this->def_map_prod_prob > nl)
                {
                    nl = this->def_map_prod_prob + log(1 + exp(nl - this->def_map_prod_prob));
                }
                else
                {
                    nl = nl + log(exp(this->def_map_prod_prob - nl) + 1);
                }
            }
            nl -= Parameter::sparse_prior;

            if (map_decomp_opt.empty() || nl > max_sum_prob)
            {
                map_decomp_opt.clear();
                max_sum_prob = nl;
                map_decomp_opt.push_back(def_map_decomp);
            }
        }

        n.log_val = max_sum_prob - log(n.cnt + 1);

        // randomly break tie
        this->map_decomps[ti] = map_decomp_opt[Utils::random_next_int(map_decomp_opt.size())];
        map_decomp_opt.clear();

        if (map_types.empty() || n.log_val > this->def_map_sum_prob)
        {
            this->def_map_sum_prob = n.log_val;
            map_types.clear();
        }
        if (n.log_val == this->def_map_sum_prob)
        {
            map_types.push_back(ti);
        }
    }

    if (chosen_blank_idx >= 0)
    {
        SumNode &n = this->types[chosen_blank_idx];
        n.log_val = this->def_map_prod_prob - log(n.cnt + 1) - Parameter::sparse_prior;
        map_decomps[chosen_blank_idx] = def_map_decomp;

        if (map_types.empty() || n.log_val > this->def_map_sum_prob)
        {
            this->def_map_sum_prob = n.log_val;
            map_types.clear();
            map_types.push_back(chosen_blank_idx);
        }
    }

    this->def_map_type_idx = map_types[Utils::random_next_int(map_types.size())];
    map_types.clear();
}

// downward track-back step
void Region::set_cur_parse_to_MAP(int inst_idx)
{
    if (this->a == 1 && this->b == 1)
        return;

    // type node
    if (this->types.size() == 1)
        this->inst_type[inst_idx] = 0;  // only one choice
    int chosen_type = this->inst_type[inst_idx];
    std::string di = this->map_decomps[chosen_type];

    this->inst_decomp[inst_idx] = di;
    Decomposition d = Decomposition::get_decomposition(di);
    Region &r1 = Region::get_region(d.region_id_1);
    Region &r2 = Region::get_region(d.region_id_2);

    r1.inst_type[inst_idx] = d.type_id_1;
    r2.inst_type[inst_idx] = d.type_id_2;

    // record update if slave
    if (!MyMPI::is_class_master && SPN::is_recording_update)
    {
        MyMPI::buf_int[MyMPI::buf_idx++] = id;
        MyMPI::buf_int[MyMPI::buf_idx++] = chosen_type;
        MyMPI::buf_int[MyMPI::buf_idx++] = d.region_id_1;
        MyMPI::buf_int[MyMPI::buf_idx++] = d.region_id_2;
        MyMPI::buf_int[MyMPI::buf_idx++] = d.type_id_1;
        MyMPI::buf_int[MyMPI::buf_idx++] = d.type_id_2;
    }

    // if product node not created, create it now
    if(this->decomp_prod.count(di) == 0)
    {
        ProdNode np = ProdNode();
        np.add_child(r1.types[d.type_id_1]);
        np.add_child(r2.types[d.type_id_2]);
        this->decomp_prod.insert(std::pair<std::string, ProdNode>(di, np));
    }

    r1.set_cur_parse_to_MAP(inst_idx);
    r2.set_cur_parse_to_MAP(inst_idx);
}

// clear an existing parse for incremental EM
void Region::clear_cur_parse(int inst_idx)
{
    if (this->inst_type.count(inst_idx) == 0)
        return;
    if(this->a == 1 && this->b == 1)
        return;
    int cti = this->inst_type[inst_idx];
    std::string di = inst_decomp[inst_idx];

    this->inst_type.erase(inst_idx);
    this->inst_decomp.erase(inst_idx);
    Decomposition d = Decomposition::get_decomposition(di);

    // record update if slave
    if (!MyMPI::is_class_master && SPN::is_recording_update)
    {
        MyMPI::buf_int[MyMPI::buf_idx++] = id;
        MyMPI::buf_int[MyMPI::buf_idx++] = cti;
        MyMPI::buf_int[MyMPI::buf_idx++] = d.region_id_1;
        MyMPI::buf_int[MyMPI::buf_idx++] = d.region_id_2;
        MyMPI::buf_int[MyMPI::buf_idx++] = d.type_id_1;
        MyMPI::buf_int[MyMPI::buf_idx++] = d.type_id_2;
    }

    Region &r1 = Region::get_region(d.region_id_1);
    r1.clear_cur_parse(inst_idx);
    Region &r2 = Region::get_region(d.region_id_2);
    r2.clear_cur_parse(inst_idx);
}

// clear parse from other slave
void Region::clear_cur_parse_from_buf(int chosen_type, int ri1, int ri2, int ti1, int ti2)
{
    if (this->a == 1 && this->b == 1)
        return;

    std::string di = Decomposition::get_id_str(ri1, ri2, ti1, ti2);
    SumNode &n = this->types[chosen_type];
    n.remove_child_only(di, 1);
}

void Region::set_cur_parse_from_buf(int chosen_type, int ri1, int ri2, int ti1, int ti2)
{
    if (this->a == 1 && this->b == 1)
        return;

    std::string di = Decomposition::get_id_str(ri1, ri2, ti1, ti2);
    SumNode &n = this->types[chosen_type];
    Decomposition d = Decomposition::get_decomposition(di);

    // if prod node not created, create it now
    ProdNode np;
    if (this->decomp_prod.count(di) == 0)
    {
        np = ProdNode();
        Region &r1 = Region::get_region(d.region_id_1);
        Region &r2 = Region::get_region(d.region_id_2);

        np.add_child(r1.types[d.type_id_1]);
        np.add_child(r2.types[d.type_id_2]);

        this->decomp_prod.insert(std::pair<std::string, ProdNode>(di, np));
    }
    else
        np = this->decomp_prod[di];
    n.add_child_only(di, 1, np);
}
