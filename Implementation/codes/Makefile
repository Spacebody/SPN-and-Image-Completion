VERSION = 0.0.4

# TODO -- Incremental Update

CC	= mpic++
FLAGS	= -g -Wall
LFLAGS	= -lm
OBJS	= parameter.o timer.o utils.o my_mpi.o \
		  instance.o node.o sum_node.o prod_node.o decomposition.o SPN.o region.o generative_learning.o \
		  dataset.o image_completion.o eval.o run.o run_main.o eval_main

TARGETS	= run eval# TODO

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

image_completion.o: $(EVAL)image_completion.cpp $(EVAL)image_completion.hpp
	$(CC) $(FLAGS) -c $(EVAL)image_completion.cpp $(EVAL)image_completion.hpp

eval.o: $(EVAL)eval.cpp $(EVAL)eval.hpp
	$(CC) $(FLAGS) -c $(EVAL)eval.cpp $(EVAL)eval.hpp

run.o: $(EVAL)run.cpp $(EVAL)run.hpp
	$(CC) $(FLAGS) -c $(EVAL)run.cpp $(EVAL)run.hpp

run_main.o: $(EVAL)run_main.cpp
	$(CC) $(FLAGS) -c $(EVAL)run_main.cpp

eval_main.o: $(EVAL)eval_main.cpp
	$(CC) $(FLAGS) -c $(EVAL)eval_main.cpp

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

# target
run: $(EVAL)run_main.cpp run.o image_completion.o generative_learning.o SPN.o node.o my_mpi.o utils.o dataset.o parameter.o sum_node.o prod_node.o timer.o decomposition.o region.o instance.o
	$(CC) $(FLAGS) $(EVAL)run_main.cpp run.o image_completion.o generative_learning.o SPN.o node.o my_mpi.o utils.o dataset.o parameter.o sum_node.o prod_node.o timer.o decomposition.o region.o instance.o -o run

eval: $(EVAL)eval_main.cpp eval.o parameter.o
	$(CC) $(FLAGS) $(EVAL)eval_main.cpp eval.o parameter.o -o eval

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
