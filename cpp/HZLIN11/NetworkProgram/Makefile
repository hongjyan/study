include Makefile.defines

SUBDIR = lib
LIB = lib/demonlib.a
SRC = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRC)) 
SERVSRC = $(wildcard tcpserv*.c) $(wildcard tcpserv*.cpp)
CLISRC = $(wildcard tcpcli*.c) $(wildcard tcpcli*.cpp)
BINARIES = $(patsubst %.c, %.bin, $(SERVSRC)) $(patsubst %.cpp, %.bin, $(SERVSRC)) $(patsubst %.c, %.bin, $(CLISRC)) $(patsubst %.cpp, %.bin, $(CLISRC))
#BINARIES = tcpserv01.bin tcpcli01.bin tcpcli02.bin tcpcli_sigpipe.bin tcpserv02.bin tcpserv03.bin tcpcli03.bin tcpcli_5_2.bin tcpserv_select.bin tcpcli_select.bin tcpserv_selectForListenSocket.bin


all : .PHONY $(OBJS) $(BINARIES) 

#caculate summary of two long. the same code with tcpcli01.c at server site.
#client: code same as tcpcli01 except client connect 5 times to server
tcpserv02.bin : tcpserv02.o
	$(CC)  $^ str_echo02.o $(LIB) -o $@

#use struct type to store two long at server&client sites. use sscanf at str_cli03.c, use readn at str_echo03.c 
#If different endian at server and client site, then go wrong.
tcpserv03.bin : tcpserv03.o
	$(CC) $^ str_echo03.o $(LIB) -o $@
tcpcli03.bin : tcpcli03.o
	$(CC) $^ str_cli03.o $(LIB) -o $@

#server: use select instead of read, make block happene at select(). Other part is the same with tcpser02.c
#client: same as tcpcli02.c
tcpserv_selectread.bin : tcpserv_selectread.o
	$(CC) $^ str_echo_selectread.o $(LIB) -o $@

#server: same as tcpserv_selectread.c
#client: use select to check if sendbuffer bigger than SNDLOWAT. Normally, nobody will use select at write side.
tcpcli_selectwrite.bin : tcpcli_selectwrite.o
	$(CPP) $^ str_cli_selectwrite.o $(LIB) -o $@

#set SNDLOWAT for write. The others same with above one. 
tcpcli_select_SNDLOWAT.bin : tcpcli_select_SNDLOWAT.o
	$(CC) $^ str_cli_select_SNDLOWAT.o $(LIB) -o $@

#server: the same as tcpserv02.c
#client: use select for socket and input, to slove the data which comes from socket won't be handled if program block at input.
tcpcli_select01.bin : tcpcli_select01.o
	$(CC) $^ strcli_select01.o $(LIB) -o $@

#server: the same as tcpserv02.c
#client: to use a file as input instead of stdin, try produce the problem decribed in book which is "tcpserver can not echo all input from client".
tcpcli_select01_IOFILE.bin : tcpcli_select01_IOFILE.o
	$(CC) $^ strcli_select01_IOFILE.o $(LIB) -o $@
#same as above one, except fix problem: return too early when fgets done from file, but there is still data transfering during client and server.
tcpcli_select02_IOFILE.bin : tcpcli_select02_IOFILE.o
	$(CC) $^ strcli_select02_IOFILE.o $(LIB) -o $@

%.bin : %.o
	$(CC) $^ $(LIB)  -o $@

%.o : %.c
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c $< -o $@

%.o : %.cpp
	$(CPP) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

.PHONY:
	make -C $(SUBDIR)
	@echo -------------------------------------------------------------
	@echo $(SERVSRC)
	@echo $(CLISRC)
	@echo -------------------------------------------------------------

clean: 
	cd $(SUBDIR) && rm -rf *.o *.a
	rm -rf *.o *.bin
