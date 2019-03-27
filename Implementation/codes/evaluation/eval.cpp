#include "eval.hpp"
#include "../common/parameter.hpp"
#include <cmath>
#include <iostream>
#include <dirent.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

std::string Eval::exp_dir = "../../Implementation";

void Eval::eval_olivetti()
{
    int size = Parameter::input_dim1, pad_len = 10;
    std::string olive_dir = Eval::exp_dir + "/results/olivetti";
    double lm = -1;
    lm = Eval::cmp_MSE_left(olive_dir + "/completions/olive-left.dat", size, pad_len);
    double bm = -1;
    bm = Eval::cmp_MSE_bottom(olive_dir + "/completions/olive-btm.dat", size, pad_len);
    std::cout << "\n\nOlivetti MSE\t Left=" + std::to_string(round(lm)) + "\tBottom=" + std::to_string(round(bm)) << std::endl;
}

void Eval::eval_caltech()
{
    std::string caltech_dir = Eval::exp_dir + "/results/caltech/completions";
    int size = 64, pad_len = 10;
    struct dirent *ptr;
    DIR *dir;
    dir = opendir(caltech_dir.c_str());
    std::vector<std::string> files = std::vector<std::string>();
    while ((ptr = readdir(dir)) != NULL)
    {
        if (ptr->d_name[0] == '.')
            continue;
        files.push_back(ptr->d_name);
    }
    closedir(dir);                         // close directory
    std::sort(files.begin(), files.end()); // sort in alphabet
    double ttl_lt_mse = 0, ttl_bm_mse = 0;
    int num_cat = 0;
    std::cout << "CAT\tLeft\tBottom" << std::endl;
    for (int di = 0; di < files.size(); ++di)
    {
        std::string fn = files[di];
        std::string::size_type idx = fn.find("-left.dat", 0);
        std::string cat = "";
        if (idx != std::string::npos)
            cat = fn.substr(0, idx);
        if (cat == "" || cat == "BACKGROUND_Google")
            continue;
        double lm = Eval::cmp_MSE_left(caltech_dir + "/" + cat + "-left.dat", size, pad_len);
        double bm = Eval::cmp_MSE_bottom(caltech_dir + "/" + cat + "-btm.dat", size, pad_len);
        ++num_cat;
        ttl_lt_mse += lm;
        ttl_bm_mse += bm;
        std::cout << cat + " " + std::to_string(round(lm)) + " " + std::to_string(round(bm)) << std::endl;
    }
    std::cout << "\nCaltech-" + std::to_string(num_cat) + "\t" + std::to_string(round(ttl_lt_mse / num_cat)) + "\t" + std::to_string(round(ttl_bm_mse / num_cat)) << std::endl;
}

double Eval::cmp_MSE_left(std::string fn, int size, int pad_len)
{
    std::ifstream in;
    in.open(fn);
    double p = 0;
    int c = 0;
    std::string s;
    int idx = 0;
    while (std::getline(in, s))
    {
        ++idx;
        if (idx == size + pad_len)
            idx = 0;
        else if (idx > size)
            continue;
        else
        {
            std::vector<std::string> ts;
            std::stringstream ss(s);
            ts.clear();
            while (std::getline(ss, s, ','))
                ts.push_back(s); // split by ','
            for (int i = 0; i < size / 2; ++i)
            {
                double q1 = std::stod(ts[i]), q2 = std::stod(ts[i + (size + pad_len)]);
                p += (q1 - q2) * (q1 - q2);
                ++c;
            }
        }
    }
    in.close();
    return p / c;
}

double Eval::cmp_MSE_bottom(std::string fn, int size, int pad_len)
{
    std::ifstream in;
    in.open(fn);
    double p = 0;
    int c = 0;
    std::string s;
    int idx = 0;
    while (std::getline(in, s))
    {
        ++idx;
        if (idx == size + pad_len)
            idx = 0;
        else if (idx > size)
            continue;
        else if (idx > size / 2)
        {
            std::vector<std::string> ts;
            std::stringstream ss(s);
            ts.clear();
            while (std::getline(ss, s, ','))
                ts.push_back(s); // split by ','
            for (int i = 0; i < size; ++i)
            {
                double q1 = std::stod(ts[i]), q2 = std::stod(ts[i + (size + pad_len)]);
                p += (q1 - q2) * (q1 - q2);
                ++c;
            }
        }
    }
    in.close();
    return p / c;
}

std::string Eval::get_exp_dir()
{
    return Eval::exp_dir;
}
