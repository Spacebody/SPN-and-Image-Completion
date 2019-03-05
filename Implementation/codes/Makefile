VERSION = 0.0.2

# TODO -- Update

CC	= mpic++
FLAGS	= -g -Wall
LFLAGS	= -lm
OBJS	= parameter.o timer.o utils.o my_mpi.o \
		  instance.o node.o sum_node.o prod_node.o
TARGETS	= #

# src path
COMMON = ./common/
EVAL = ./evaluation/
SPN = ./spn/

# make all test program
all: $(TARGETS)

# recompile obj if edited
edit: $(OBJS)

# compile objects
## for common
parameter.o: $(COMMON)parameter.cpp $(COMMON)parameter.hpp
	$(CC) $(FLAGS) -c $(COMMON)parameter.cpp $(COMMON)parameter.hpp

timer.o: $(COMMON)timer.cpp $(COMMON)timer.hpp
	$(CC) $(FLAGS) -c $(COMMON)timer.cpp $(COMMON)timer.hpp

utils.o: $(COMMON)utils.cpp $(COMMON)utils.hpp
	$(CC) $(FLAGS) -c $(COMMON)utils.cpp $(COMMON)utils.hpp

my_mpi.o: $(COMMON)my_mpi.cpp $(COMMON)my_mpi.hpp
	$(CC) $(FLAGS) -c $(COMMON)my_mpi.cpp $(COMMON)my_mpi.hpp

## for evaluation

## for spn
instance.o: $(SPN)instance.cpp $(SPN)instance.hpp
	$(CC) $(FLAGS) -c $(SPN)instance.cpp $(SPN)instance.hpp

node.o: $(SPN)node.cpp $(SPN)node.hpp
	$(CC) $(FLAGS) -c $(SPN)node.cpp $(SPN)node.hpp

sum_node.o: $(SPN)sum_node.cpp $(SPN)sum_node.hpp
	$(CC) $(FLAGS) -c $(SPN)sum_node.cpp $(SPN)sum_node.hpp

prod_node.o: $(SPN)prod_node.cpp $(SPN)prod_node.hpp
	$(CC) $(FLAGS) -c $(SPN)prod_node.cpp $(SPN)prod_node.hpp

# clean
.PHONY:clean all
clean:
	rm -rf *.o *.out *.dSYM ./common/*.gch ./spn/*.gch ./evaluation/*.gch
