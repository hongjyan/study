CXX = g++
CXXFLAGS = -Wall -std=c++11
INCFLAGS = -Iinclude -Iinclude/common -Iinclude/func
APP = a.out

SRCS = $(shell find src -name '*.cpp')
OBJS = $(patsubst %.cpp, %.o, $(SRCS))

all: $(APP)

$(APP): $(OBJS) main.o 
	$(CXX) $(CXXFLAGS) $(INCFLAGS) main.o $(OBJS) -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCFLAGS)  -c $< -o $@
clean:
	rm -rf $(OBJS) a.out main.o
