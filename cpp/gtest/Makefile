GTEST_INCLUDE = /var/fpwork/hongjyan/nrm/has/nrm-dir/sdk/sysroots/core2-64-pc-linux-gnu/usr/include/gtest-fused
GTEST_LIB =  gtest
GTEST_LIB_DIR = ?
LD_FLAGS = -L ${GTEST_LIB_DIR} -l${GTEST_LIB} -lpthread
COMPILE_FLAGS = -c -ggdb -Wall -std=c++11 -I${GTEST_INCLUDE}


TARGET = GTESTPRACTICE

%.o : %.cpp
	g++ ${COMPILE_FLAGS} $<

OBJECTS =  main.o gtestPracticeTests.o


all: ${TARGET}


${TARGET} :  ${OBJECTS}
	g++ -o ${TARGET} ${OBJECTS}

clean:
	rm -rf *.o ${TARGET}

.PHONY: all  clean
