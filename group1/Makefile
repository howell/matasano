CC=gcc
CFLAGS=-Wall -fstrict-aliasing -Wstrict-aliasing -g

SRCS=main.c \
	 convert.c convert.h \
	 xor.c xor.h

OBJFILE=test.o

test: $(SRCS)
	$(CC) -o $(OBJFILE) $(CFLAGS) $(SRCS)

clean:
	rm -rf $(OBJFILE) $(OBJFILE).dSYM
