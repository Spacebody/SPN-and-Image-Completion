#include "my_mpi.hpp"
#include "parameter.hpp"
#include "utils.hpp"

// master
int MyMPI::rank = 0;
bool MyMPI::is_class_master = false;
int MyMPI::master_rank = -1;
int MyMPI::my_slave = -1;  // class master

// slave
int MyMPI::my_offset = -1;  // slave
int MyMPI::my_start = -1;   // slave

void MyMPI::set_constants_for_imgs()
{
    MyMPI::is_class_master = (MyMPI::rank == 0);
    MyMPI::master_rank = 0;

    MyMPI::my_offset = MyMPI::rank - 1; // slave
    MyMPI::my_start = 1;                // slave
    MyMPI::my_slave = 1;                // class master

    set_random_seed_by_rank();
}

void MyMPI::set_constants_for_imgs_parallel()
{
    MyMPI::is_class_master = (MyMPI::rank % (Parameter::num_slave_per_class + 1) == 0);

    // slave
    MyMPI::my_offset = (MyMPI::rank - 1) % (Parameter::num_slave_per_class + 1);
    MyMPI::my_start = MyMPI::rank - MyMPI::my_offset;
    MyMPI::master_rank = MyMPI::rank - MyMPI::my_offset - 1;

    // master
    MyMPI::my_slave = MyMPI::rank + 1;
    MyMPI::set_random_seed_by_rank();
}

void MyMPI::set_random_seed_by_rank()
{
    Utils::seed = MyMPI::rank;
    Utils::generator = std::default_random_engine(Utils::seed);
}

// buffer
int MyMPI::buf_idx = 0;
int MyMPI::buf_size = 10000000;
std::vector<int> MyMPI::buf_int = std::vector<int>(MyMPI::buf_size);
std::vector<double> MyMPI::buf_double = std::vector<double>(100);
std::vector<char> MyMPI::buf_char = std::vector<char>(100);

// MPI util
double MyMPI::recv_double(int src, int tag)
{
    MPI_Recv(&MyMPI::buf_double, 1, MPI_DOUBLE, src, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return MyMPI::buf_double[0];
}

void MyMPI::send_double(int dest, int tag, double d)
{
    MyMPI::buf_double[0] = d;
    MPI_Send(&MyMPI::buf_double, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
}

char MyMPI::recv_char(int src, int tag)
{
    MPI_Recv(&MyMPI::buf_char, 1, MPI_CHAR, src, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return MyMPI::buf_char[0];
}

void MyMPI::send_char(int dest, int tag, char c)
{
    MyMPI::buf_char[0] = c;
    MPI_Send(&MyMPI::buf_char, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
}
