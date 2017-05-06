CXX = g++ 
AR = ar rvs

TCPSRCS = $(shell find common tcp -name '*.cpp')
TCPOBJS = $(patsubst %.cpp, %.o, $(TCPSRCS))
TESTSRCS = $(shell find APP -name '*.cpp')
TESTOBJS = $(patsubst %.cpp, %.o, $(TESTSRCS))
LIB = tcpproto.a
APP = app.bin

INCFLAGS = -Icommon -Itcp
CXXFLAGS = -Wall -std=c++11
LDDFLAGS = -lpthread
DEBUGFLAGS = -DDEBUG

all : $(APP) 

$(APP) : $(TESTOBJS) $(LIB) 
	$(CXX) $^ -lpthread -o $@

$(LIB) : $(TCPOBJS)
	$(AR) $@ $^

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCFLAGS) $(DEBUGFLAGS) -c $< -o $@
		
clean:
	rm -rf $(TCPOBJS) $(TESTOBJS) $(LIB) $(APP)
