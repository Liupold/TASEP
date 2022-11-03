MAKEFLAGS := --jobs=$(shell nproc)

CC = gcc
CFLAGS = -march=native -O2 -Wall -Wextra -Wpedantic -Ofast -fopenmp -Wconversion \
		 -std=c99 -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual \
		 -Wstrict-prototypes -Wdeclaration-after-statement# -Werror
DIR_SRC = src
DIR_LIB = lib
DIR_BINs = bins
DIR_INCLUDE = include
src_progs := $(shell find $(DIR_SRC) -name '*.c' | sed "s/.*\//$(DIR_BINs)\//; s/\.c//")

src: $(DIR_BINs) $(src_progs)

$(DIR_LIB)/libtasep.so: $(DIR_LIB)/tasep.c
	$(CC) $(CFLAGS) -I$(DIR_INCLUDE) -shared $(DIR_LIB)/tasep.c -o $(DIR_LIB)/libtasep.so

$(DIR_BINs)/%: $(DIR_SRC)/%.c $(DIR_LIB)/libtasep.so
	$(CC) $(CFLAGS) -I$(DIR_INCLUDE) -L$(DIR_LIB) $< -l:libtasep.so -Wl,-rpath=$(realpath $(DIR_LIB)) -o $@

$(DIR_BINs):
	mkdir -p $(DIR_BINs)

clean:
	rm -rf $(DIR_BUILD) $(DIR_BINs)
	rm $(DIR_LIB)/libtasep.so
