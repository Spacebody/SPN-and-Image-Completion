VERSION = 0.0.4

# TODO -- Incremental Update

CC	= mpic++
FLAGS	= -g -Wall
LFLAGS	= -lm
OBJS	= parameter.o timer.o utils.o my_mpi.o \
		  instance.o node.o sum_node.o prod_node.o decomposition.o SPN.o region.o generative_learning.o \
		  dataset.o

TARGETS	= # TODO

# src path
COMMON = ./common/
EVAL = ./evaluation/
SPN = ./spn/

.PHONY:all clean cleanobj cleandebug cleangch alltest
# make all test program
all: $(TARGETS)

# make all test and only left tests
alltest: $(TARGETS)
	rm -rf *.o *.out *.dSYM ./*/*.gch

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
dataset.o: $(EVAL)dataset.cpp $(EVAL)dataset.hpp
	$(CC) $(FLAGS) -c $(EVAL)dataset.cpp $(EVAL)dataset.hpp

## for spn
instance.o: $(SPN)instance.cpp $(SPN)instance.hpp
	$(CC) $(FLAGS) -c $(SPN)instance.cpp $(SPN)instance.hpp

node.o: $(SPN)node.cpp $(SPN)node.hpp
	$(CC) $(FLAGS) -c $(SPN)node.cpp $(SPN)node.hpp

sum_node.o: $(SPN)sum_node.cpp $(SPN)sum_node.hpp
	$(CC) $(FLAGS) -c $(SPN)sum_node.cpp $(SPN)sum_node.hpp

prod_node.o: $(SPN)prod_node.cpp $(SPN)prod_node.hpp
	$(CC) $(FLAGS) -c $(SPN)prod_node.cpp $(SPN)prod_node.hpp

decomposition.o: $(SPN)decomposition.cpp $(SPN)decomposition.hpp
	$(CC) $(FLAGS) -c $(SPN)decomposition.cpp $(SPN)decomposition.hpp

SPN.o: $(SPN)SPN.cpp $(SPN)SPN.hpp
	$(CC) $(FLAGS) -c $(SPN)SPN.cpp $(SPN)SPN.hpp

region.o: $(SPN)region.cpp $(SPN)region.hpp
	$(CC) $(FLAGS) -c $(SPN)region.cpp $(SPN)region.hpp

generative_learning.o: $(SPN)generative_learning.cpp $(SPN)generative_learning.hpp
	$(CC) $(FLAGS) -c $(SPN)generative_learning.cpp $(SPN)generative_learning.hpp

# clean
clean:
	rm -rf *.o *.out *.dSYM ./*/*.gch
cleanobj:
	rm -rf *.o
cleandebug:
	rm -rf *.dSYM
cleangch:
	rm -rf ./*/*.gch
cleanbin:
	rm -rf $(TARGETS)
