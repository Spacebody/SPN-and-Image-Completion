#include "run.hpp"
#include "../common/parameter.hpp"
#include <mpi.h>
#include <string.h>
#include "../common/my_mpi.hpp"
#include "../common/utils.hpp"

// domain
std::string Run::DOM_OLIVETTI = "O";
std::string Run::DOM_CALTECH = "C";

// directory
std::string Run::Run::exp_dir = "../../../Implementation";

std::string Run::olive_data_dir = Run::exp_dir + "/data/olivetti";
std::string Run::olive_rst_data = Run::exp_dir + "/results/olivetti/completions";
std::string Run::olive_md_data = Run::exp_dir + "/results/olivetti/models";

std::string Run::cal_data_dir = Run::exp_dir + "/data/caltech";
std::string Run::cal_rst_dir = Run::exp_dir + "results/caltech/completions";
std::string Run::cal_md_dir = Run::exp_dir + "results/caltech/models";

static void proc(int argc, char *argv[])
{
    Run::proc(argc, argv);
    if (Parameter::domain == Run::DOM_OLIVETTI)
    {
        Run::run_olivetti();
    }
    else if (Parameter::domain == Run::DOM_CALTECH)
    {
        Run::run_caltech();
    }
    MPI::Finalize();
}

static void proc_args(int argc, char *argv[])
{
    for (int i = 0; i < argc; ++i)
    {
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
            std::string prompt = "\n\nOptions: [-d <Domain>]\n\t\t [-sp <Sparse Prior>]\n\t\t [-br <Base Resolution>]\n\t\t [-ncv <Num Components per Var>]\n\t\t [-nsr <Num Sum per Region>]\n\t\t [-ct <Convergency Threhold>]\n\t\t [-bs <Batch Size>]\n\t\t [-ns <Num Slave per cat>]\n\t\t [-nsg <Num Slave Grp>]";
            Utils::println(prompt);
            exit(0);
        }  
    }
}

static void run_caltech()
{

}

static std::map<std::string, std::map<std::string, std::string> > get_caltech_info()
{
    int my_id = MyMPI::rank / (Parameter::num_slave_per_class + 1);
    std::map<std::string, std::map<std::string, std::string> > cat_info = Run::get_caltech_info();
    int num_cat = -1;
    for (std::string cat; )
    {
        
    }
}

static void run_olivetti()
{

}
