#ifndef RUN_HPP
#define RUN_HPP

#include <string>
#include <map>

class Run
{
    

    // directory
    static std::string exp_dir;
    static std::string olive_data_dir;
    static std::string olive_rst_data;
    static std::string olive_md_data;

    static std::string cal_data_dir;
    static std::string cal_rst_dir;
    static std::string cal_md_dir;
    
    static void proc_args(int argc, char *argv[]);

  public:
    // domain
    static std::string DOM_OLIVETTI;
    static std::string DOM_CALTECH;

    static void proc(int argc, char *argv[]);

    static void run_olivetti();
    static void run_caltech();
    static std::map<std::string, std::map<std::string, std::string> > get_caltech_info();
};

#endif