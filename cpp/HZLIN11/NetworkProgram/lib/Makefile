include ../Makefile.defines

SRCS = $(wildcard *.c)
objects := $(patsubst %.c, %.o, $(SRCS))
packagename = demonlib.a

All : $(objects)
	$(AR) -rv $(packagename) $(objects)
%.o : %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean :
	rm -rf *.o *.a

