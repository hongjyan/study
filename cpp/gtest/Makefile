GTEST_INCLUDE = /var/fpwork/hongjyan/nrm/has/nrm-dir/esdk/tmp/work/core2-64-pc-linux-gnu/gtest/1.7.0-r0/sysroot-destdir/usr/include/gtest-fused
LD_LIBRARY_PATH =/var/fpwork/hongjyan/nrm/has/nrm-dir/esdk/tmp/sysroots/qemux86-64
GTEST_LIB =  gtest
GTEST_LIB_DIR = /var/fpwork/hongjyan/nrm/has/nrm-dir/esdk/tmp/sysroots/qemux86-64/usr/lib64 
LD_FLAGS = -L${GTEST_LIB_DIR} -l${GTEST_LIB} -lpthread
COMPILE_FLAGS = -c -ggdb -Wall -std=c++11 -I${GTEST_INCLUDE}

#LDPATH=--dynamic-linker=/var/fpwork/hongjyan/nrm/has/nrm-dir/esdk/tmp/sysroots/qemux86-64/lib64/ld-linux-x86-64.so.2 -Wl,-rpath=/var/fpwork/hongjyan/nrm/has/nrm-dir/esdk/tmp/sysroots/qemux86-64/lib64


TARGET = GTESTPRACTICE

%.o : %.cpp
	g++ ${COMPILE_FLAGS} $<

OBJECTS =  main.o gtestPracticeTests.o


all: ${TARGET}


${TARGET} :  ${OBJECTS}
	g++ -o  $(LD_FLAGS) ${LDPATH} --dynamic-linker=/var/fpwork/hongjyan/nrm/has/nrm-dir/esdk/tmp/sysroots/qemux86-64/lib64/ld-linux-x86-64.so.2 -rpath=/var/fpwork/hongjyan/nrm/has/nrm-dir/esdk/tmp/sysroots/qemux86-64/lib64 ${TARGET} ${OBJECTS} 

clean:
	rm -rf *.o ${TARGET}

.PHONY: all  clean
