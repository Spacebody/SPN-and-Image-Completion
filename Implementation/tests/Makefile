#
# In order to execute this "Makefile" just type "make"
#
# -g option enables debugging mode 
# -c flag generates object code for separate files

CC	= mpic++
FLAGS	= -g -Wall
LFLAGS	= -lm
OBJS	= parameter.o timer.o utils.o my_mpi.o \
		  instance.o node.o sum_node.o prod_node.o SPN.o decomposition.o region.o generative_learning.o \
		  dataset.o

TARGETS	= test_parameter test_timer test_my_mpi test_utils \
		  test_instance test_node test_sum_node test_prod_node test_decomposition test_SPN test_region test_generative_learning \
		  test_dataset

# test path
TP_COMMON = ./common/
TP_EVAL = ./evaluation/
TP_SPN = ./spn/

# src path
SP_COMMON = ../codes/common/
SP_EVAL = ../codes/evaluation/
SP_SPN = ../codes/spn/

# PHONY
.PHONY:all clean cleanobj cleandebug cleangch alltest

# make all test program
all: $(TARGETS)

# make all test and only left tests
alltest: $(TARGETS)
	rm -rf *.o *.out *.dSYM ../codes/*/*.gch

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
dataset.o: $(SP_EVAL)dataset.cpp $(SP_EVAL)dataset.hpp
	$(CC) $(FLAGS) -c $(SP_EVAL)dataset.cpp $(SP_EVAL)dataset.hpp

## for spn
instance.o: $(SP_SPN)instance.cpp $(SP_SPN)instance.hpp
	$(CC) $(FLAGS) -c $(SP_SPN)instance.cpp $(SP_SPN)instance.hpp

node.o: $(SP_SPN)node.cpp $(SP_SPN)node.hpp
	$(CC) $(FLAGS) -c $(SP_SPN)node.cpp $(SP_SPN)node.hpp

sum_node.o: $(SP_SPN)sum_node.cpp $(SP_SPN)sum_node.hpp
	$(CC) $(FLAGS) -c $(SP_SPN)sum_node.cpp $(SP_SPN)sum_node.hpp

prod_node.o: $(SP_SPN)prod_node.cpp $(SP_SPN)prod_node.hpp
	$(CC) $(FLAGS) -c $(SP_SPN)prod_node.cpp $(SP_SPN)prod_node.hpp

decomposition.o: $(SP_SPN)decomposition.cpp $(SP_SPN)decomposition.hpp
	$(CC) $(FLAGS) -c $(SP_SPN)decomposition.cpp $(SP_SPN)decomposition.hpp

SPN.o: $(SP_SPN)SPN.cpp $(SP_SPN)SPN.hpp
	$(CC) $(FLAGS) -c $(SP_SPN)SPN.cpp $(SP_SPN)SPN.hpp

region.o: $(SP_SPN)region.cpp $(SP_SPN)region.hpp
	$(CC) $(FLAGS) -c $(SP_SPN)region.cpp $(SP_SPN)region.hpp

generative_learning.o: $(SP_SPN)generative_learning.cpp $(SP_SPN)generative_learning.hpp
	$(CC) $(FLAGS) -c $(SP_SPN)generative_learning.cpp $(SP_SPN)generative_learning.hpp

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
test_dataset: $(TP_EVAL)test_dataset.cpp dataset.o parameter.o instance.o my_mpi.o utils.o timer.o
	$(CC) $(FLAGS) $(TP_EVAL)test_dataset.cpp dataset.o parameter.o instance.o my_mpi.o utils.o timer.o -o test_dataset

## for spn
test_instance: $(TP_SPN)test_instance.cpp instance.o
	$(CC) $(FLAGS) $(TP_SPN)test_instance.cpp instance.o -o test_instance

test_node: $(TP_SPN)test_node.cpp node.o
	$(CC) $(FLAGS) $(TP_SPN)test_node.cpp node.o -o test_node

test_sum_node: $(TP_SPN)test_sum_node.cpp sum_node.o node.o parameter.o utils.o my_mpi.o timer.o
	$(CC) $(FLAGS) $(TP_SPN)test_sum_node.cpp sum_node.o node.o parameter.o utils.o my_mpi.o timer.o -o test_sum_node

test_prod_node: $(TP_SPN)test_prod_node.cpp prod_node.o node.o parameter.o utils.o my_mpi.o timer.o
	$(CC) $(FLAGS) $(TP_SPN)test_prod_node.cpp prod_node.o node.o parameter.o utils.o my_mpi.o timer.o -o test_prod_node

test_decomposition: $(TP_SPN)test_decomposition.cpp decomposition.o 
	$(CC) $(FLAGS) $(TP_SPN)test_decomposition.cpp decomposition.o -o test_decomposition

test_SPN: $(TP_SPN)test_SPN.cpp SPN.o instance.o decomposition.o node.o sum_node.o prod_node.o region.o parameter.o utils.o my_mpi.o timer.o 
	$(CC) $(FLAGS) $(TP_SPN)test_SPN.cpp SPN.o instance.o decomposition.o node.o sum_node.o prod_node.o region.o parameter.o utils.o my_mpi.o timer.o -o test_SPN

test_region: $(TP_SPN)test_region.cpp region.o sum_node.o prod_node.o parameter.o instance.o utils.o decomposition.o my_mpi.o SPN.o node.o timer.o
	$(CC) $(FLAGS) $(TP_SPN)test_region.cpp region.o sum_node.o prod_node.o parameter.o instance.o utils.o decomposition.o my_mpi.o SPN.o node.o timer.o -o test_region

test_generative_learning: $(TP_SPN)test_generative_learning.cpp generative_learning.o SPN.o instance.o my_mpi.o utils.o parameter.o node.o sum_node.o prod_node.o region.o decomposition.o timer.o
	$(CC) $(FLAGS) $(TP_SPN)test_generative_learning.cpp generative_learning.o SPN.o instance.o my_mpi.o utils.o parameter.o node.o sum_node.o prod_node.o region.o decomposition.o timer.o -o test_generative_learning

# clean files
clean:
	rm -rf *.o *.out *.dSYM ../codes/*/*.gch $(TARGETS)
cleanobj:
	rm -rf *.o
cleandebug:
	rm -rf *.dSYM
cleangch:
	rm -rf ../codes/*/*.gch
cleanbin:
	rm -rf $(TARGETS)
