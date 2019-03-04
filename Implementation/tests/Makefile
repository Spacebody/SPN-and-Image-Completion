#
# In order to execute this "Makefile" just type "make"
#
# -g option enables debugging mode 
# -c flag generates object code for separate files

CC	= mpic++
FLAGS	= -g -Wall
LFLAGS	= -lm
OBJS	= parameter.o timer.o utils.o my_mpi.o
TARGETS	= test_parameter test_timer test_my_mpi test_utils

# test path
TP_COMMON = ./common/
TP_EVAL = ./evaluation/
TP_SPN = ./spn/

# src path
SP_COMMON = ../codes/common/
SP_EVAL = ../codes/evaluation/
SP_SPN = ../codes/spn/

# make all test program
all: test_parameter test_timer test_my_mpi test_utils

# recompile obj if edited
edit: parameter.o timer.o

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
 
instance.o: $(SP_SPN)instance.cpp $(SP_SPN)instance.hpp
	$(CC) $(FLAGS) -c $(SP_SPN)instance.cpp $(SP_SPN)instance.hpp

## for evaluation

## for spn

# create/compile the individual files >>separately<<
test_parameter: $(TP_COMMON)test_parameter.cpp parameter.o
	$(CC) $(FLAGS) $(TP_COMMON)test_parameter.cpp parameter.o -o test_parameter

test_my_mpi: $(TP_COMMON)test_my_mpi.cpp parameter.o utils.o timer.o my_mpi.o instance.o
	$(CC) $(FLAGS) $(TP_COMMON)test_my_mpi.cpp parameter.o utils.o timer.o my_mpi.o instance.o -o test_my_mpi

test_timer: $(TP_COMMON)test_timer.cpp timer.o
	$(CC) $(FLAGS) $(TP_COMMON)test_timer.cpp timer.o -o test_timer

test_utils: $(TP_COMMON)test_utils.cpp utils.o timer.o my_mpi.o parameter.o instance.o 
	$(CC) $(FLAGS) $(TP_COMMON)test_utils.cpp utils.o timer.o my_mpi.o parameter.o instance.o -o test_utils


# clean
.PHONY:clean all
clean:
	rm -rf *.o *.out *.dSYM ../codes/common/*.gch ../codes/spn/*.gch ../codes/evaluation/*.gch $(TARGETS)

