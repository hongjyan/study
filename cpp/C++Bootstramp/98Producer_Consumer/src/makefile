SRC = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRC))
BINARY = ../a.out

all : $(BINARY)

$(BINARY) : $(OBJS)
	g++ -Wall -ggdb $(OBJS) -lpthread -o $(BINARY)

%.o : %.cpp
	g++ -Wall -ggdb  -DDEBUG=1  -I../include -c $^ -o $@

clean:
	rm -rf *.o *.out
