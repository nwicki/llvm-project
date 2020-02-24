PATH:=$(PATH):../build/bin
CC=clang++
CFLAGS=-fsanitize=dataflow

all: $(patsubst %.cpp, %.out, $(wildcard *.cpp))

%.out: %.cpp Makefile
	$(CC) $(CFLAGS) $< -o $@
	./$@
	rm $@