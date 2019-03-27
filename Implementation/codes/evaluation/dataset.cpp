#include "dataset.hpp"
#include <dirent.h>
#include "../common/utils.hpp"
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <iterator>

std::vector<std::vector<int> > Dataset::tmp = std::vector<std::vector<int> >(Parameter::input_dim1, std::vector<int>(Parameter::input_dim2, 0));

std::string Dataset::exp_dir = "..";
std::string Dataset::olivetti_raw_file_name = Dataset::exp_dir + "/data/olivetti/olivetti.raw";
std::string Dataset::cal_data_dir = Dataset::exp_dir + "/data/caltech";
std::string Dataset::cal_rst_dir = Dataset::exp_dir + "/results/caltech/completions";
std::string Dataset::cal_mdl_dir = Dataset::exp_dir +"/results/caltech/models";

int Dataset::RESCALE_LEN = 100;

Dataset::Dataset()
{
    this->train = std::vector<Instance>();
    this->test = std::vector<Instance>();
}

// dataset
std::vector<Instance> Dataset::get_train()
{
    return this->train;
}

std::vector<Instance> Dataset::get_test()
{
    return this->test;
}

// divide train/test
std::set<int> Dataset::gen_test_idx(int max_size, int test_size)
{
    std::set<int> tis = std::set<int>();
    for (int i = max_size - test_size; i < max_size; ++i)
    {
        tis.insert(i);
        if (tis.size() == test_size)
            break;
    }
    return tis;
}

void Dataset::set_instance(std::vector<std::vector<int> > buf, Instance &inst)
{
    double tf = 0, varf = 0, cf = 0;
    for (int i = 0; i < Parameter::input_dim1; ++i)
        for (int j = 0; j < Parameter::input_dim2; ++j)
        {
            tf += buf[i][j];
            varf += buf[i][j] * buf[i][j];
            ++cf;
        }
    tf /= cf;
    varf /= cf;
    inst.mean = tf;
    inst.std = sqrt(varf - tf * tf);
    inst.vals = std::vector<std::vector<double> >(Parameter::input_dim1, std::vector<double>(Parameter::input_dim2, 0.0));
    for (int i = 0; i < Parameter::input_dim1; ++i)
        for (int j = 0; j < Parameter::input_dim2; ++j)
        {
            inst.vals[i][j] = (buf[i][j] - inst.mean) / inst.std;
        }
}

//---------------------------------------------------
// Caltech
//---------------------------------------------------
void Dataset::load_caltech(std::string dir_name)
{
    struct dirent *ptr;
    DIR *dir;
    std::string dir_path = Dataset::cal_data_dir + "/" + dir_name;
    std::cout << "Loading " + dir_path << std::endl;
    dir = opendir(dir_path.c_str());
    std::vector<std::string> files = std::vector<std::string>();
    while ((ptr = readdir(dir)) != NULL)
    {
        if (ptr->d_name[0] == '.')
            continue;
        files.push_back(ptr->d_name);
    }
    closedir(dir);  // close directory
    std::sort(files.begin(), files.end()); // sort in alphabet
    int max_size = files.size();
    int test_size = max_size / 3;
    if (test_size > Parameter::max_test_size)
        test_size = Parameter::max_test_size;
    this->train = std::vector<Instance>();
    this->test = std::vector<Instance>();
    for (int i = 0; i < files.size(); ++i)
    {
        Instance inst = Dataset::read_cal_instance(dir_path + "/" + files[i]);
        if (i < max_size - test_size)
            this->train.push_back(inst);
        else
            this->test.push_back(inst);
    }
    if (!MyMPI::is_class_master && MyMPI::my_offset == 0)
        Utils::println(dir_name + ": train.size=" + std::to_string(this->train.size()) + " test.size=" + std::to_string(this->test.size()));
}

Instance Dataset::read_cal_instance(std::string fn)
{
    int delta = (Dataset::RESCALE_LEN - Parameter::input_dim1) / 2;
    Instance inst;
    std::ifstream in;
    in.open(fn, std::fstream::in);
    std::string s;
    std::vector<std::string> ts;
    int idx = 0;
    while (std::getline(in, s))
    {
        s = Utils::trim(s);
        if (s.length() == 0)
            continue;
        std::stringstream ss(s); // split by ' '
        ts = std::vector<std::string>((std::istream_iterator<std::string>(ss)), std::istream_iterator<std::string>());
        int true_idx = idx - delta;
        if (true_idx >= 0 && true_idx < Parameter::input_dim1)
            for (int k = 0; k < Parameter::input_dim2; ++k)
            {
                int p = (int)std::stod(ts[k + delta]);
                Dataset::tmp[true_idx][k] = p;
            }
            ++idx;
    }
    in.close();
    Dataset::set_instance(Dataset::tmp, inst);
    return inst;
}

//---------------------------------------------------
// Olivetti
//---------------------------------------------------
void Dataset::load_olivetti()
{
    std::set<int> tis = Dataset::gen_test_idx(400, Parameter::max_test_size);
    std::ifstream in;
    in.open(Dataset::olivetti_raw_file_name, std::fstream::in);
    std::vector<std::vector<double> > faces = std::vector<std::vector<double> >(4096, std::vector<double>(400, 0.0));
    std::string s;
    std::vector<std::string> ts;
    int idx = 0;
    while (std::getline(in, s))
    {
        s = Utils::trim(s);
        if (s.length() == 0)
            continue;
        std::stringstream ss(s); // split by ' '
        ts = std::vector<std::string>((std::istream_iterator<std::string>(ss)), std::istream_iterator<std::string>());
        for (int i = 0; i < 400; ++i)
            faces[idx][i] = std::stod(ts[i]);
        ++idx;
    }
    in.close();

    this->train.clear();
    this->test.clear();
    for (int pi = 0; pi < 400; ++pi)
    {
        Instance inst = Dataset::read_olivetti_instance(faces, pi);
        if (tis.count(pi))
            this->test.push_back(inst);
        else
            this->train.push_back(inst);
    }
}

Instance Dataset::read_olivetti_instance(std::vector<std::vector<double> > faces, int pi)
{
    Instance inst;
    for (int i = 0; i < Parameter::input_dim1; ++i)
        for (int j = 0; j < Parameter::input_dim2; ++j)
        {
            int k = j * Parameter::input_dim1 + i;
            Dataset::tmp[i][j] = (int)faces[k][pi];
        }
    Dataset::set_instance(Dataset::tmp, inst);
    return inst;
}
