CXX = g++
CXXFLAGS = -Wall
INCFLAGS = -I.
LXXFLAGS = -lprotobuf


PROTOS = $(shell find  . -name '*.proto')
PROTOSRCS = $(patsubst %.proto, %.pb.cc, $(PROTOS))
PROTOOBJS = $(patsubst %.pb.cc, %.pb.o, $(PROTOSRCS))
PROTOPATH = .
OBJS = main_read.o main_write.o
EXECUTE_READ = main_read
EXECUTE_WRITE = main_write

all : $(PROTOOBJS) $(EXECUTE_READ) $(EXECUTE_WRITE)

$(PROTOOBJS) : $(PROTOSRCS)

%.pb.cc : %.proto
	protoc --proto_path=$(PROTOPATH) --cpp_out=$(PROTOPATH) $(PROTOS)

%.pb.o : %.pb.cc
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $^ -o $@

$(EXECUTE_READ): main_read.o
	$(CXX) $(LXXFLAGS) $(PROTOOBJS) $^ -o $@

$(EXECUTE_WRITE): main_write.o
	$(CXX) $(LXXFLAGS) $(PROTOOBJS) $^ -o $@
	
%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $^ -o $@

clean:
	rm -rf *.pb.cc *.pb.h *.o $(EXECUTE_READ) $(EXECUTE_WRITE)
