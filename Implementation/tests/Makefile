#
# In order to execute this "Makefile" just type "make"
#
# -g option enables debugging mode 
# -c flag generates object code for separate files

CC	= mpic++
FLAGS	= -g -Wall
LFLAGS	= -lm

OBJ_COMMON = parameter.o timer.o utils.o my_mpi.o
OBJ_SPN = instance.o node.o sum_node.o prod_node.o SPN.o decomposition.o region.o generative_learning.o
OBJS	= $(OBJ_COMMON) $(OBJ_SPN) dataset.o image_completion.o eval.o run.o

TARGETS	= test_parameter test_timer test_my_mpi test_utils \
		  test_instance test_node test_sum_node test_prod_node test_decomposition test_SPN test_region test_generative_learning \
		  test_dataset test_image_completion test_eval test_run

# src path
VPATH = ../codes/common:../codes/evaluation:../codes/spn:./common:./evaluation:./spn

# PHONY
.PHONY:all clean cleanobj cleandebug cleangch alltest

# make all test program
all: $(TARGETS)

# make all test and only left tests
alltest: $(TARGETS)
	rm -rf *.o *.out *.dSYM ../codes/*/*.gch

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

# create/compile the individual files separately
## for common
test_parameter: test_parameter.cpp parameter.o
	$(CC) $(FLAGS) $^ -o $@

test_my_mpi: test_my_mpi.cpp $(OBJ_COMMON) instance.o
	$(CC) $(FLAGS) $^ -o $@

test_timer: test_timer.cpp timer.o
	$(CC) $(FLAGS) $^ -o $@

test_utils: test_utils.cpp $(OBJ_COMMON) instance.o 
	$(CC) $(FLAGS) $^ -o $@

## for evaluation
test_dataset: test_dataset.cpp dataset.o instance.o $(OBJ_COMMON) 
	$(CC) $(FLAGS) $^ -o $@

test_image_completion: test_image_completion.cpp image_completion.o $(OBJ_COMMON) SPN.o instance.o sum_node.o prod_node.o node.o decomposition.o region.o  
	$(CC) $(FLAGS) $^ -o $@

test_eval: test_eval.cpp eval.o parameter.o
	$(CC) $(FLAGS) $^ -o $@

test_run: test_run.cpp run.o $(OBJ_COMMON) dataset.o image_completion.o $(OBJ_SPN)
	$(CC) $(FLAGS) $^ -o $@

## for spn
test_instance: test_instance.cpp instance.o
	$(CC) $(FLAGS) $^ -o $@

test_node: test_node.cpp node.o
	$(CC) $(FLAGS) $^ -o $@

test_sum_node: test_sum_node.cpp sum_node.o node.o $(OBJ_COMMON)
	$(CC) $(FLAGS) $^ -o $@

test_prod_node: test_prod_node.cpp prod_node.o node.o $(OBJ_COMMON)
	$(CC) $(FLAGS) $^ -o $@

test_decomposition: test_decomposition.cpp decomposition.o 
	$(CC) $(FLAGS) $^ -o $@

test_SPN: test_SPN.cpp SPN.o instance.o decomposition.o node.o sum_node.o prod_node.o region.o $(OBJ_COMMON) 
	$(CC) $(FLAGS) $^ -o $@

test_region: test_region.cpp region.o sum_node.o prod_node.o instance.o decomposition.o SPN.o node.o $(OBJ_COMMON)
	$(CC) $(FLAGS) $^ -o $@

test_generative_learning: test_generative_learning.cpp $(OBJ_SPN) $(OBJ_COMMON)
	$(CC) $(FLAGS) $^ -o $@

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
