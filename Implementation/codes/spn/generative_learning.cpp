#include "generative_learning.hpp"

#include "../common/my_mpi.hpp"
#include "../common/utils.hpp"
#include <chrono> // for getting system time in milliseconds
#include "../common/parameter.hpp"
#include <cmath>

SPN GenerativeLearning::get_SPN()
{
    return this->spn;
}

void GenerativeLearning::learn(std::vector<Instance> train)
{
    this->learn_hard_EM(train);
}

void GenerativeLearning::save_model(std::string mdl_file_name)
{
    this->spn.save_DSPN(mdl_file_name);
}

void GenerativeLearning::learn_hard_EM(std::vector<Instance> train)
{
    this->spn.training_set = train;
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    bool is_log = false;

    this->spn.print_params();
    this->spn.init();
    std::string msg = "init";
    Utils::log_time_ms(msg);

    //------------------------------------------------------
    //-----process each mini-batch, find map, update weights
    //------------------------------------------------------
    double ollh = 0;
    double ori_prior = Parameter::sparse_prior;

    int num_inst_per_slave = (int)ceil(Parameter::batch_size * 1.0 / Parameter::num_slave_per_class);

    for (int iter = 1; iter <= Parameter::max_iter; ++iter)
    {
        // anneal prior
        if (iter <= 10)
            Parameter::sparse_prior = ori_prior * iter / 10;

        for (int bi = 0; bi < train.size(); bi += Parameter::batch_size)
        {
            // master: aggregate update and pass on
            if (MyMPI::is_class_master)
            {
                // recv clear-parse
                MyMPI::buf_idx = 0;
                for (int i = 0; i < Parameter::num_slave_per_class; ++i)
                {
                    if (i*num_inst_per_slave < Parameter::batch_size && bi + i*num_inst_per_slave < train.size())
                    {
                        if (is_log)
                            Utils::println("recv clear update from" + std::to_string((MyMPI::my_slave + i)));
                        this->spn.recv_update(MyMPI::my_slave + i);
                    }
                }

                for (int i = 0; i < Parameter::num_slave_per_class; ++i)
                {
                    if (is_log)
                        Utils::println("send clear update from" + std::to_string((MyMPI::my_slave + i)));
                    this->spn.send_update(MyMPI::my_slave + i);
                }

                // recv parse from slaves
                MyMPI::buf_idx = 0;
                for (int i = 0; i < Parameter::num_slave_per_class; ++i)
                {
                    if (i * num_inst_per_slave < Parameter::batch_size && bi + i * num_inst_per_slave < train.size())
                    {
                        if (is_log)
                            Utils::println("recv parse update from" + std::to_string((MyMPI::my_slave + i)));
                        this->spn.recv_update(MyMPI::my_slave + i);
                    }
                }

                for (int i = 0; i < Parameter::num_slave_per_class; ++i)
                {
                    if (is_log)
                        Utils::println("send parse update from" + std::to_string((MyMPI::my_slave + i)));
                    this->spn.send_update(MyMPI::my_slave + i);
                }
            }
            else  // slave
            {
                int k = MyMPI::my_offset;
                if (k * num_inst_per_slave < Parameter::batch_size && bi + k*num_inst_per_slave < train.size())
                {
                    MyMPI::buf_idx = 0;
                    for (int i = k * num_inst_per_slave; i < (k + 1) * num_inst_per_slave && bi + i < train.size(); ++i)
                    {
                        // map -> update cnt
                        this->spn.clear_cur_parse(bi + i);
                    }
                    if (is_log)
                        Utils::println("send clear update to " + std::to_string(MyMPI::master_rank));
                    this->spn.send_update(MyMPI::master_rank);
                }

                if (is_log)
                    Utils::println("recv clear update from " + std::to_string(MyMPI::master_rank));

                MyMPI::buf_idx = 0;
                this->spn.recv_update(MyMPI::master_rank);
                this->spn.clear_cur_parse_from_buf();

                if (is_log)
                {
                    std::string msg = "clear parse";
                    Utils::log_time_ms(msg);
                }

                if (k * num_inst_per_slave < Parameter::batch_size && bi + k * num_inst_per_slave < train.size())
                {
                    MyMPI::buf_idx = 0;
                    for (int i = k * num_inst_per_slave; i < (k + 1) * num_inst_per_slave && bi + i < train.size(); ++i)
                    {
                        // map -> update cnt
                        this->spn.infer_MAP_for_learning(bi + i, train[bi+i]);
                        this->spn.set_cur_parse_to_MAP(bi + i);
                    }
                    if (is_log)
                        Utils::println("send parse update to " + std::to_string(MyMPI::master_rank));
                    this->spn.send_update(MyMPI::master_rank);
                }

                if (is_log)
                {
                    std::string msg = "cmp map ...";
                    Utils::log_time_ms(msg);
                }

                MyMPI::buf_idx = 0;
                this->spn.recv_update(MyMPI::master_rank);
                this->spn.set_cur_parse_from_buf();

                if (is_log)
                {
                    std::string msg = "update weight";
                    Utils::log_time_ms(msg);
                }
            }
        }
        std::string msg = "finish iter " + std::to_string(iter);
        Utils::log_time_ms(msg);

        if(!MyMPI::is_class_master)
            this->spn.clear_unused_in_SPN();
        
        // convergence test
        if (MyMPI::is_class_master)
        {
            double llh = 0;
            for (int i = 0; i < Parameter::num_slave_per_class; ++i)
            {
                int rank = i + MyMPI::my_slave;
                llh += this->recv_llh(rank);
            }
            llh /= train.size();
            std::string msg = "[iter=" + std::to_string(iter) + "] llh=" + std::to_string(llh) + " ollh=" + std::to_string(ollh);
            Utils::log_time_ms(msg);
            if (iter == 1)
                ollh = llh;
            else
            {
                double dllh = abs(llh - ollh);
                ollh = llh;
                if (dllh < Parameter::threshold_LLHChg)
                {
                    Utils::println("\tllh converged");
                    
                    // send msg break
                    for (int k = 0; k < Parameter::num_slave_per_class; ++k)
                        this->send_msg_break(k + MyMPI::my_slave);
                    break;
                }
            }

            // send_msg OK
            for (int k = 0; k < Parameter::num_slave_per_class; ++k)
                this->send_msg_OK(k + MyMPI::my_slave);
        }
        else
        {
            // slave: compute llh, send to class master
            double llh = 0;
            int size = (int)ceil(train.size() * 1.0 / Parameter::num_slave_per_class);
            for (int i = MyMPI::my_offset * size; i < (MyMPI::my_offset + 1) * size && i < train.size(); i++)
            {
                Instance inst = train[i];
                llh += this->spn.llh(inst);
            }
            this->send_llh(MyMPI::master_rank, llh);

            // recv msg: continue?
            char msg = this->recv_msg(MyMPI::master_rank);
            if (msg == 'B')
                break;
        }

        if (is_log)
        {
            std::string msg = "done with convergence test";
            Utils::log_time_ms(msg);
        }            
    }

    // time
    std::chrono::duration<double, std::milli> time_span = std::chrono::high_resolution_clock::now() - start_time;
    Utils::println("Total learning time: " + std::to_string(time_span.count()));
}

//--------------------------------------------------
// Utils
//--------------------------------------------------
void GenerativeLearning::send_msg_break(int dest)
{
    MyMPI::send_char(dest, 1, 'B');
}

void GenerativeLearning::send_msg_OK(int dest)
{
    MyMPI::send_char(dest, 1, 'O');
}
char GenerativeLearning::recv_msg(int src)
{
    return MyMPI::recv_char(src, 1);
}

void GenerativeLearning::send_llh(int dest, double d)
{
    int tag = 1;
    MyMPI::send_double(dest, tag, d);
}

double GenerativeLearning::recv_llh(int src)
{
    int tag = 1;
    return MyMPI::recv_double(src, tag);
}
