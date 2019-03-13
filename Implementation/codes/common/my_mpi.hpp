#ifndef MYMPI_HPP
#define MYMPI_HPP

#include <mpi.h>
#include <random>
#include <vector>

class MyMPI
{
  public:
    MyMPI()
    {
        MyMPI::rank = MPI::COMM_WORLD.Get_rank();
    }
    ~MyMPI() {}
    MyMPI(const MyMPI &my_mpi) {}
    MyMPI &operator=(const MyMPI &my_mpi) { return *this; }

    // master
    static int rank;
    static bool is_class_master;
    static int master_rank;
    static int my_slave;
    // slave
    static int my_offset;
    static int my_start;

    static void set_constants_for_imgs();
    static void set_constants_for_imgs_parallel();
    static void set_random_seed_by_rank();

    // buffer
    static int buf_idx;
    static int buf_size;
    static std::vector<int> buf_int;
    static std::vector<double> buf_double;
    static std::vector<char> buf_char;

    // MPI util
    static double recv_double(int src, int tag);
    static void send_double(int dest, int tag, double d);
    static char recv_char(int src, int tag);
    static void send_char(int dest, int tag, char c);
};

#endif