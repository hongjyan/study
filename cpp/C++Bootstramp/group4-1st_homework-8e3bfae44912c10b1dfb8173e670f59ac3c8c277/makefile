####################################################################
#
#       $File: Makefile $
#
#       Copyright (c) Nokia Networks 2017
#
####################################################################
#
#       Makefile for Cstring
#
####################################################################
#
#       $Id$
#
####################################################################
CC:=g++
CFLAGS := -g -Wall -O3
CPPFLAGS := -std=c++11

INCFLAGS := -I./
SRCS := $(wildcard *.cpp)
HEADERS := $(wildcard *.hpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

EXECUTABLE := cstring

################### Target #####################################

.PHONY : all clean

all : $(EXECUTABLE)

$(EXECUTABLE) : $(OBJS)
	$(CC) -o $(EXECUTABLE) $(OBJS)

$(OBJS) : $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCFLAGS) -c $(SRCS)

clean :
	rm -f *.o

veryclean : clean
	rm -f $(EXECUTABLE)
    

### END OF FILE ###
