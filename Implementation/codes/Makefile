VERSION = 1.0.0

CC	= mpic++
FLAGS	= -g -Wall
LFLAGS	= -lm

OBJ_COMMON = parameter.o timer.o utils.o my_mpi.o
OBJ_SPN = instance.o node.o sum_node.o prod_node.o SPN.o decomposition.o region.o generative_learning.o
OBJS	= $(OBJ_COMMON) $(OBJ_SPN) dataset.o image_completion.o eval.o run.o run_main.o eval_main

TARGETS	= run eval# TODO

# src path
VPATH = ./common:./evaluation:./spn

.PHONY:all clean cleanobj cleandebug cleangch alltest
# make all test program
all: $(TARGETS)

# make all test and only left tests
alltest: $(TARGETS)
	rm -rf *.o *.out *.dSYM ./*/*.gch

# compile objects
## for common
parameter.o: parameter.cpp parameter.hpp
	$(CC) $(FLAGS) -c $^

timer.o: timer.cpp timer.hpp
	$(CC) $(FLAGS) -c $^

utils.o: utils.cpp utils.hpp
	$(CC) $(FLAGS) -c $^

my_mpi.o: my_mpi.cpp my_mpi.hpp
	$(CC) $(FLAGS) -c $^

## for evaluation
dataset.o: dataset.cpp dataset.hpp
	$(CC) $(FLAGS) -c $^

image_completion.o: image_completion.cpp image_completion.hpp
	$(CC) $(FLAGS) -c $^

eval.o: eval.cpp eval.hpp
	$(CC) $(FLAGS) -c $^

run.o: run.cpp run.hpp
	$(CC) $(FLAGS) -c $^

run_main.o: run_main.cpp
	$(CC) $(FLAGS) -c $^

eval_main.o: eval_main.cpp
	$(CC) $(FLAGS) -c $^

## for spn
instance.o: instance.cpp instance.hpp
	$(CC) $(FLAGS) -c $^

node.o: node.cpp node.hpp
	$(CC) $(FLAGS) -c $^

sum_node.o: sum_node.cpp sum_node.hpp
	$(CC) $(FLAGS) -c $^

prod_node.o: prod_node.cpp prod_node.hpp
	$(CC) $(FLAGS) -c $^

decomposition.o: decomposition.cpp decomposition.hpp
	$(CC) $(FLAGS) -c $^

SPN.o: SPN.cpp SPN.hpp
	$(CC) $(FLAGS) -c $^

region.o: region.cpp region.hpp
	$(CC) $(FLAGS) -c $^

generative_learning.o: generative_learning.cpp generative_learning.hpp
	$(CC) $(FLAGS) -c $^

# target
run: run_main.cpp run.o image_completion.o dataset.o $(OBJ_COMMON) $(OBJ_SPN)
	$(CC) $(FLAGS) $^ -o $@

eval: eval_main.cpp eval.o parameter.o
	$(CC) $(FLAGS) $^ -o $@

# clean
clean:
	rm -rf *.o *.out *.dSYM ./*/*.gch $(TARGETS)
cleanobj:
	rm -rf *.o
cleandebug:
	rm -rf *.dSYM
cleangch:
	rm -rf ./*/*.gch
cleanbin:
	rm -rf $(TARGETS)
