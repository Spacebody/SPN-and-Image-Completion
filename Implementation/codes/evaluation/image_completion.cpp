#include "image_completion.hpp"
#include <cmath>
#include <iostream>

int ImageCompletion::PAD_LEN = 10;

void ImageCompletion::output_rst_to_img(std::fstream &out, int inst_idx, Instance &inst)
{
    // output original and completed images size by size
    int dim1 = (int)inst.vals.size(), dim2 = (int)inst.vals[0].size();
    int sz = dim1 * dim2;
    for (int ri = 0; ri < dim1; ++ri)
    {
        int off = ri * dim2;

        // original
        for (int ci = 0; ci < dim2; ++ci)
        {
            int v = Utils::get_int_val(inst, inst.vals[ri][ci]);
            if (ci > 0)
                out << ',';
            out << "" + std::to_string(v);
        }
        
        // pad
        for (int i = 0; i < ImageCompletion::PAD_LEN; ++i)
            out << ",0";

        // completion
        for (int ci = 0; ci < dim2; ++ci)
        {
            out << "," + std::to_string(MyMPI::buf_int[MyMPI::buf_idx + off + ci]);
        }

        out << "\n";
    }

    // pad
    for (int k = 0; k < ImageCompletion::PAD_LEN; ++k)
    {
        out << "0";
        int len = dim2 * 2 + ImageCompletion::PAD_LEN;
        for (int i = 1; i < len; ++i)
        {
            out << ",0";
        }
        out << "\n";
    }
    MyMPI::buf_idx += sz;
}

void ImageCompletion::send_img(int dest)
{
    MPI_Send(MyMPI::buf_int, MyMPI::buf_idx, MPI_INT, dest, 0, MPI_COMM_WORLD);
}

int ImageCompletion::recv_img(int src)
{
    MPI_Status status;
    MPI_Recv(MyMPI::buf_int, MyMPI::buf_size, MPI_INT, src, 0, MPI_COMM_WORLD, &status);
    int count;
    MPI_Get_count(&status, MPI_INT, &count);
    return count;
}

void ImageCompletion::complete_left(std::vector<Instance> test, std::string mdl_dir, std::string mdl_name, std::string rst_dir)
{
    Utils::println("complete left half for mdl=" + mdl_dir + "/" + mdl_name);
    SPN spn = SPN::load_DSPN(mdl_dir + "/" + mdl_name + ".mdl");
    ImageCompletion::complete_left(spn, test, mdl_name, rst_dir);
}

void ImageCompletion::complete_left(SPN spn, std::vector<Instance> test, std::string mdl_name, std::string rst_dir)
{
    Utils::println("---> complete left half and output " + rst_dir + "/" + mdl_name + "-left.dat");
    int size = (int)ceil((int)test.size() * 1.0 / Parameter::num_slave_per_class);

    if (!MyMPI::is_class_master)
    {
        int master = MyMPI::master_rank;
        MyMPI::buf_idx = 0;
        for (int i = MyMPI::my_offset * size; i < (int)test.size() && i < (MyMPI::my_offset + 1) * size; ++i)
        {
            Instance &inst = test[i];
            spn.complete_left_img(inst);
        }
        ImageCompletion::send_img(master);
    }
    else
    {
        std::fstream out(rst_dir + "/" + mdl_name + "-left.dat", std::fstream::out);
        for (int si = 1; si <= Parameter::num_slave_per_class; ++si)
        {
            int src = si + MyMPI::rank;
            recv_img(src);
            MyMPI::buf_idx = 0;
            for (int i = (si - 1) * size; i < (int)test.size() && i < si * size; ++i)
            {
                Instance &inst = test[i];
                ImageCompletion::output_rst_to_img(out, i, inst);
            }
        }
        out.close();
    }
}

void ImageCompletion::complete_bottom(std::vector<Instance> test, std::string mdl_dir, std::string mdl_name, std::string rst_dir)
{
    Utils::println("complete bottom half and output" + mdl_dir + "/" + mdl_name);
    SPN spn = SPN::load_DSPN(mdl_dir + "/" + mdl_name + ".mdl");
    ImageCompletion::complete_bottom(spn, test, mdl_name, rst_dir);
}

void ImageCompletion::complete_bottom(SPN spn, std::vector<Instance> test, std::string mdl_name, std::string rst_dir)
{
    Utils::println("---> complete bottom half and output " + rst_dir + "/" + mdl_name + "-btm.dat");
    int size = (int)ceil((int)test.size() * 1.0 / Parameter::num_slave_per_class);

    if (!MyMPI::is_class_master)
    {
        int master = MyMPI::master_rank;
        MyMPI::buf_idx = 0;
        for (int i = MyMPI::my_offset * size; i < (int)test.size() && i < (MyMPI::my_offset + 1) * size; ++i)
        {
            Instance &inst = test[i];
            spn.complete_bottom_img(inst);
        }
        ImageCompletion::send_img(master);
    }
    else
    {
        std::fstream out(rst_dir + "/" + mdl_name + "-btm.dat", std::fstream::out);
        for (int si = 1; si <= Parameter::num_slave_per_class; ++si)
        {
            int src = si + MyMPI::rank;
            recv_img(src);
            MyMPI::buf_idx = 0;
            for (int i = (si - 1) * size; i < (int)test.size() && i < si * size; ++i)
            {
                Instance &inst = test[i];
                ImageCompletion::output_rst_to_img(out, i, inst);
            }
        }
        out.close();
    }
}

int ImageCompletion::get_pad_len()
{
    return this->PAD_LEN;
}
