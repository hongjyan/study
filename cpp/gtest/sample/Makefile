# Makefile for gtest examples

GOOGLE_TEST_LIB = gtest
GOOGLE_TEST_INCLUDE = /var/fpwork/hongjyan/nrm/has/nrm-dir/sdk/sysroots/core2-64-pc-linux-gnu/usr/include/gtest-fused

G++ = g++
G++_FLAGS = -c -Wall -std=c++11 -I $(GOOGLE_TEST_INCLUDE)
#LD_FLAGS = -L /usr/local/lib -l $(GOOGLE_TEST_LIB) -l pthread

OBJECTS = main.o string-compare.o
TARGET = string-compare

all: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ -o $(TARGET) $(OBJECTS) $(LD_FLAGS)

%.o : %.cpp
	$(G++) $(G++_FLAGS) $<

clean:
	rm -f $(TARGET) $(OBJECTS)
                    
.PHONY: all clean
