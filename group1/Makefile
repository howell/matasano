CC=clang
CLANGFLAGS=-x c -Wall -Wextra -std=c99 -g -O2
GCCFLAGS=-Wall -fstrict-aliasing -Wstrict-aliasing -std=c99 -g -O2

SRCS=main.c \
	 convert.c convert.h \
	 xor.c xor.h \
	 text_score.c text_score.h

OBJFILE=test.o

test: $(SRCS)
	$(CC) -o $(OBJFILE) $(CLANGFLAGS) $(SRCS)

clean:
	rm -rf $(OBJFILE) $(OBJFILE).dSYM
