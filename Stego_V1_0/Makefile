CC=gcc
CFLAGS=-std=c99 -Wall -O0 -static
CLIBS=-lm
PRGNAME=Stego_V1_0
CFILES=$(shell find . -name '*.c')
HFILES=$(shell find . -name '*.h')
STYLE=astyle --style=1tbs
RUN=valgrind --leak-check=full
DEBUG=gdb --args
ARGS=

all: build

clean:
	$(STYLE) $(CFILES)
	rm -f *.o
	rm -f $(PRGNAME)

build:
	$(CC) $(CFLAGS) $(CLIBS) -o $(PRGNAME) -Isrc $(CFILES)

debug:
	$(CC) $(CFLAGS) -g $(CLIBS) -o $(PRGNAME) -Isrc $(CFILES)

run_debug:
	$(DEBUG) ./$(PRGNAME) $(ARGS)

run:
	./$(PRGNAME) $(ARGS)

memtest:
	$(RUN) ./$(PRGNAME) $(ARGS)
