CC = gcc
CFLAGS = -Wall 
LFLAGS = -lzmq -Wl,-rpath -Wl,/usr/local/lib/
INFLAGS = -I. -I/usr/include/

SRCS = $(shell find . -name '*.c')
OBJS = $(patsubst %.c, %.o, $(SRCS))


BINARYS = hello_server.bin world_client.bin

$(BINARYS) : $(OBJS)
	$(CC) $(LFLAGS) hello_server.o -o hello_server.bin
	$(CC) $(LFLAGS) world_client.o -o world_client.bin

%.o : %.c
	$(CC) $(CFLAGS) $(INFLAGS) -c $^ -o $@
	
clean:
	rm -rf $(OBJS) $(BINARYS)