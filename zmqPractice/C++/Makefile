CXX = g++
CXXFLAGS = -Wall 
LXXFLAGS = -lzmq -Wl,-rpath -Wl,/usr/local/lib/
INFLAGS = -I. -I/usr/include/

SRCS = $(shell find . -name '*.cpp')
OBJS = $(patsubst %.cpp, %.o, $(SRCS))


BINARYS = hello_server.bin world_client.bin

$(BINARYS) : $(OBJS)
	$(CXX) $(LXXFLAGS) hello_server.o -o hello_server.bin
	$(CXX) $(LXXFLAGS) world_client.o -o world_client.bin

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INFLAGS) -c $^ -o $@
	
clean:
	rm -rf $(OBJS) $(BINARYS)