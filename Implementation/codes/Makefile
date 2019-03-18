VERSION = 1.0.2

CXX	= mpic++
FLAGS	= -Wall -std=c++11 
LFLAGS	= -lm

OBJ_COMMON = parameter.o timer.o utils.o my_mpi.o
OBJ_SPN = instance.o node.o sum_node.o prod_node.o SPN.o decomposition.o region.o generative_learning.o
OBJS	= $(OBJ_COMMON) $(OBJ_SPN) dataset.o image_completion.o eval.o run.o run_main.o eval_main

TARGETS	= run eval

# src path
VPATH = ./common:./evaluation:./spn

.PHONY:all clean cleanobj cleangch alltest
# make all test program
all: $(TARGETS)

# make all test and only left tests
alltest: $(TARGETS)
	rm -rf *.o *.out ./*/*.gch

# compile objects
## for common
parameter.o: parameter.cpp parameter.hpp
	$(CXX) $(FLAGS) -c $^

timer.o: timer.cpp timer.hpp
	$(CXX) $(FLAGS) -c $^

utils.o: utils.cpp utils.hpp
	$(CXX) $(FLAGS) -c $^

my_mpi.o: my_mpi.cpp my_mpi.hpp
	$(CXX) $(FLAGS) -c $^

## for evaluation
dataset.o: dataset.cpp dataset.hpp
	$(CXX) $(FLAGS) -c $^

image_completion.o: image_completion.cpp image_completion.hpp
	$(CXX) $(FLAGS) -c $^

eval.o: eval.cpp eval.hpp
	$(CXX) $(FLAGS) -c $^

run.o: run.cpp run.hpp
	$(CXX) $(FLAGS) -c $^

run_main.o: run_main.cpp
	$(CXX) $(FLAGS) -c $^

eval_main.o: eval_main.cpp
	$(CXX) $(FLAGS) -c $^

## for spn
instance.o: instance.cpp instance.hpp
	$(CXX) $(FLAGS) -c $^

node.o: node.cpp node.hpp
	$(CXX) $(FLAGS) -c $^

sum_node.o: sum_node.cpp sum_node.hpp
	$(CXX) $(FLAGS) -c $^

prod_node.o: prod_node.cpp prod_node.hpp
	$(CXX) $(FLAGS) -c $^

decomposition.o: decomposition.cpp decomposition.hpp
	$(CXX) $(FLAGS) -c $^

SPN.o: SPN.cpp SPN.hpp
	$(CXX) $(FLAGS) -c $^

region.o: region.cpp region.hpp
	$(CXX) $(FLAGS) -c $^

generative_learning.o: generative_learning.cpp generative_learning.hpp
	$(CXX) $(FLAGS) -c $^

# target
run: run_main.cpp run.o image_completion.o dataset.o $(OBJ_COMMON) $(OBJ_SPN)
	$(CXX) $(FLAGS) $^ -o $@

eval: eval_main.cpp eval.o parameter.o
	$(CXX) $(FLAGS) $^ -o $@

# clean
clean:
	rm -rf *.o *.out ./*/*.gch $(TARGETS)
cleanobj:
	rm -rf *.o
cleangch:
	rm -rf ./*/*.gch
cleanbin:
	rm -rf $(TARGETS)
