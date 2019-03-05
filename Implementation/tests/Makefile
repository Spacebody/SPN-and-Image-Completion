#
# In order to execute this "Makefile" just type "make"
#
# -g option enables debugging mode 
# -c flag generates object code for separate files

CC	= mpic++
FLAGS	= -g -Wall
LFLAGS	= -lm
OBJS	= parameter.o timer.o utils.o my_mpi.o \
		  instance.o node.o sum_node.o prod_node.o
TARGETS	= test_parameter test_timer test_my_mpi test_utils \
		  test_instance test_node test_sum_node test_prod_node

# test path
TP_COMMON = ./common/
TP_EVAL = ./evaluation/
TP_SPN = ./spn/

# src path
SP_COMMON = ../codes/common/
SP_EVAL = ../codes/evaluation/
SP_SPN = ../codes/spn/

# make all test program
all: $(TARGETS)

# recompile obj if edited
edit: $(OBJS)

# compile objects
## for common
parameter.o: $(SP_COMMON)parameter.cpp $(SP_COMMON)parameter.hpp
	$(CC) $(FLAGS) -c $(SP_COMMON)parameter.cpp $(SP_COMMON)parameter.hpp

timer.o: $(SP_COMMON)timer.cpp $(SP_COMMON)timer.hpp
	$(CC) $(FLAGS) -c $(SP_COMMON)timer.cpp $(SP_COMMON)timer.hpp

utils.o: $(SP_COMMON)utils.cpp $(SP_COMMON)utils.hpp
	$(CC) $(FLAGS) -c $(SP_COMMON)utils.cpp $(SP_COMMON)utils.hpp

my_mpi.o: $(SP_COMMON)my_mpi.cpp $(SP_COMMON)my_mpi.hpp
	$(CC) $(FLAGS) -c $(SP_COMMON)my_mpi.cpp $(SP_COMMON)my_mpi.hpp

## for evaluation

## for spn
instance.o: $(SP_SPN)instance.cpp $(SP_SPN)instance.hpp
	$(CC) $(FLAGS) -c $(SP_SPN)instance.cpp $(SP_SPN)instance.hpp

node.o: $(SP_SPN)node.cpp $(SP_SPN)node.hpp
	$(CC) $(FLAGS) -c $(SP_SPN)node.cpp $(SP_SPN)node.hpp

sum_node.o: $(SP_SPN)sum_node.cpp $(SP_SPN)sum_node.hpp
	$(CC) $(FLAGS) -c $(SP_SPN)sum_node.cpp $(SP_SPN)sum_node.hpp

prod_node.o: $(SP_SPN)prod_node.cpp $(SP_SPN)prod_node.hpp
	$(CC) $(FLAGS) -c $(SP_SPN)prod_node.cpp $(SP_SPN)prod_node.hpp

# create/compile the individual files separately
## for common
test_parameter: $(TP_COMMON)test_parameter.cpp parameter.o
	$(CC) $(FLAGS) $(TP_COMMON)test_parameter.cpp parameter.o -o test_parameter

test_my_mpi: $(TP_COMMON)test_my_mpi.cpp parameter.o utils.o timer.o my_mpi.o instance.o
	$(CC) $(FLAGS) $(TP_COMMON)test_my_mpi.cpp parameter.o utils.o timer.o my_mpi.o instance.o -o test_my_mpi

test_timer: $(TP_COMMON)test_timer.cpp timer.o
	$(CC) $(FLAGS) $(TP_COMMON)test_timer.cpp timer.o -o test_timer

test_utils: $(TP_COMMON)test_utils.cpp utils.o timer.o my_mpi.o parameter.o instance.o 
	$(CC) $(FLAGS) $(TP_COMMON)test_utils.cpp utils.o timer.o my_mpi.o parameter.o instance.o -o test_utils

## for evaluation

## for spn
test_instance: $(TP_SPN)test_instance.cpp instance.o
	$(CC) $(FLAGS) $(TP_SPN)test_instance.cpp instance.o -o test_instance

test_node: $(TP_SPN)test_node.cpp node.o
	$(CC) $(FLAGS) $(TP_SPN)test_node.cpp node.o -o test_node

test_sum_node: $(TP_SPN)test_sum_node.cpp sum_node.o node.o parameter.o utils.o my_mpi.o timer.o
	$(CC) $(FLAGS) $(TP_SPN)test_sum_node.cpp sum_node.o node.o parameter.o utils.o my_mpi.o timer.o -o test_sum_node

test_prod_node: $(TP_SPN)test_prod_node.cpp prod_node.o node.o parameter.o utils.o my_mpi.o timer.o
	$(CC) $(FLAGS) $(TP_SPN)test_prod_node.cpp prod_node.o node.o parameter.o utils.o my_mpi.o timer.o -o test_prod_node


# clean
.PHONY:clean all
clean:
	rm -rf *.o *.out *.dSYM ../codes/common/*.gch ../codes/spn/*.gch ../codes/evaluation/*.gch $(TARGETS)
