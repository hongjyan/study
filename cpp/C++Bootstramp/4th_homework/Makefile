CXX = g++ 
AR = ar rvs

COMMONSRCS = $(shell find common -name '*.cpp')
COMMONOBJS = $(patsubst %.cpp, %.o, $(COMMONSRCS))
TCPSRCS = $(shell find tcp -name '*.cpp')
TCPOBJS = $(patsubst %.cpp, %.o, $(TCPSRCS))
ZMQSRCS = $(shell find zmq -name '*.cpp')
ZMQOBJS = $(patsubst %.cpp, %.o, $(ZMQSRCS))

APPSRCS = $(shell find APP -name '*.cpp')
APPOBJS = $(patsubst %.cpp, %.o, $(APPSRCS))
TCPLIB = tcpproto.a 
ZMQLIB = zmqproto.a
APP = app.bin

INCFLAGS = -Icommon -Itcp -Izmq
CXXFLAGS = -Wall -std=c++11
LDDFLAGS = -lpthread
ZMQFLAGS = -DZMQ_BUILD_DRAFT_API=1
#DEBUGFLAGS = -DDEBUG

all : $(APP) 

$(APP) : $(COMMONOBJS) $(APPOBJS) $(TCPLIB) $(ZMQLIB)
	$(CXX) $^ -lpthread -lzmq -o $@

$(TCPLIB) : $(TCPOBJS)
	$(AR) $@ $^

$(ZMQLIB) : $(ZMQOBJS)
	$(AR) $@ $^


%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCFLAGS) $(ZMQFLAGS) $(DEBUGFLAGS) -c $< -o $@
		
clean:
	rm -rf $(APPOBJS) $(COMMONOBJS) $(TCPOBJS) $(ZMQOBJS) $(PROTOLIB) $(TCPLIB) $(ZMQLIB) $(APP)
