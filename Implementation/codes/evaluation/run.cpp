#include "run.hpp"
#include "../common/parameter.hpp"
#include <mpi.h>
#include <string.h>
#include "../common/my_mpi.hpp"
#include "../common/utils.hpp"
#include "dataset.hpp"
#include "../spn/generative_learning.hpp"
#include "image_completion.hpp"
#include <dirent.h>

// domain
std::string Run::DOM_OLIVETTI = "O";
std::string Run::DOM_CALTECH = "C";

// directory
std::string Run::Run::exp_dir = "../../Implementation";

std::string Run::olive_data_dir = Run::exp_dir + "/data/olivetti";
std::string Run::olive_rst_dir = Run::exp_dir + "/results/olivetti/completions";
std::string Run::olive_mdl_dir = Run::exp_dir + "/results/olivetti/models";

std::string Run::cal_data_dir = Run::exp_dir + "/data/caltech";
std::string Run::cal_rst_dir = Run::exp_dir + "/results/caltech/completions";
std::string Run::cal_mdl_dir = Run::exp_dir + "/results/caltech/models";

void Run::proc(int argc, char *argv[])
{
    Run::proc_args(argc, argv);
    if (Parameter::domain == Run::DOM_OLIVETTI)
    {
        Run::run_olivetti();
    }
    else if (Parameter::domain == Run::DOM_CALTECH)
    {
        Run::run_caltech();
    }
}

void Run::proc_args(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {  // first argument is the name of the program
        if (strcmp(argv[i], "-d") == 0)
        {
            Parameter::domain = argv[++i];
        }
        else if (strcmp(argv[i], "-ncv") == 0)
        {
            Parameter::num_components_per_var = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-nsr") == 0)
        {
            Parameter::num_sum_per_region = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-sp") == 0)
        {
            Parameter::sparse_prior = std::stod(argv[++i]);
        }
        else if (strcmp(argv[i], "-br") == 0)
        {
            Parameter::base_resolution = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-ct") == 0)
        {
            Parameter::threshold_LLHChg = std::stod(argv[++i]);
        }
        else if (strcmp(argv[i], "-bs") == 0)
        {
            Parameter::batch_size = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-ns") == 0)
        {
            Parameter::num_slave_per_class = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-nsg"))
        {
            Parameter::num_slave_grp = std::stoi(argv[++i]);
        }
    }

    if (Parameter::domain == "")
    {
        if (MyMPI::rank == 0)
        {
            std::string prompt = "\n\nOptions: [-d <Domain>]\n\t [-sp <Sparse Prior>]\n\t [-br <Base Resolution>]\n\t [-ncv <Num Components per Var>]\n\t [-nsr <Num Sum per Region>]\n\t [-ct <Convergency Threhold>]\n\t [-bs <Batch Size>]\n\t [-ns <Num Slave per cat>]\n\t [-nsg <Num Slave Grp>]";
            Utils::println(prompt);
            exit(0);
        }  
    }
}

void Run::run_caltech()
{
    int my_id = MyMPI::rank / (Parameter::num_slave_per_class + 1);
    std::map<std::string, std::map<std::string, std::string> > cat_info = Run::get_caltech_info();
    int num_cat = -1;
    std::map<std::string, std::map<std::string, std::string> >::iterator iter;
    for (iter = cat_info.begin(); iter != cat_info.end(); ++iter)
    {
        ++num_cat;
        if (num_cat % Parameter::num_slave_grp != my_id)
            continue;
        MyMPI::set_constants_for_imgs_parallel();

        Utils::println("learn " + iter->first + " num_cat=" + std::to_string(num_cat));
        Dataset data;
        data.load_caltech(iter->first);
        // learn
        GenerativeLearning l;
        l.learn(data.get_train());
        std::cout << "Finish learning." << std::endl;
        SPN dspn = l.get_DSPN();
        if (MyMPI::my_offset == 0)
            dspn.save_DSPN(Run::cal_mdl_dir + "/" + iter->first);
        std::string log_msg = "unsup learn for completion done: " + iter->first;
        Utils::log_time_ms(log_msg);

        // complete left/bottom
        ImageCompletion::complete_left(dspn, data.get_test(), iter->first, Run::cal_rst_dir);
        ImageCompletion::complete_bottom(dspn, data.get_test(), iter->first, Run::cal_rst_dir);
    }
}

std::map<std::string, std::map<std::string, std::string> > Run::get_caltech_info()
{
    // cat - <key, val>
    std::map<std::string, std::map<std::string, std::string> > cat_info = std::map<std::string, std::map<std::string, std::string> >();
    struct dirent *ptr;
    DIR *dir;
    dir = opendir(Run::cal_data_dir.c_str());
    while ((ptr = readdir(dir)) != NULL)
    {
        if (ptr->d_name[0] == '.')
            continue;
        if (ptr->d_type == DT_DIR)
            cat_info.insert(std::pair<std::string, std::map<std::string, std::string> >(ptr->d_name, std::map<std::string, std::string>()));
    }
    closedir(dir);  // close directory
    return cat_info;
}

void Run::run_olivetti()
{
    MyMPI::set_constants_for_imgs();
    Dataset data;
    data.load_olivetti();

    // learn
    GenerativeLearning l;
    l.learn(data.get_train());
    SPN dspn = l.get_DSPN();
    if (MyMPI::my_offset == 0)
    {
        dspn.save_DSPN(Run::olive_mdl_dir + "/olive");
    }

    // complete
    ImageCompletion::complete_left(dspn, data.get_test(), "olive", Run::olive_rst_dir);
    ImageCompletion::complete_bottom(dspn, data.get_train(), "olive", Run::olive_rst_dir);
}
