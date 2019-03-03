#
# In order to execute this "Makefile" just type "make"
#

CC	 = g++
FLAGS	 = -g -c -Wall -O3
LFLAGS	 = -lm
# -g option enables debugging mode 
# -c flag generates object code for separate files


all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)


# create/compile the individual files >>separately<<
test_parameter: ./common/test_parameter.cpp ../codes/common/parameter.cpp
	$(CC) $(FLAGS) ./common/test_parameter.cpp ../codes/common/parameter.cpp

# clean
.PHONY:clean all
clean:
	rm -f *.o
