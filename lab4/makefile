CC = gcc
CFLAGS = -I ./lib -g -O -Wall
LIB = lib/t_stat.c lib/t_stat.h

OBJ := obj/main.o obj/t_stat.o
TOPDIR := $(PWD)
OBJDIR := $(TOPDIR)/obj
BINDIR := $(TOPDIR)/bin
BIN := main

SUBDIR := src
OBJHEAD := $(TOPDIR)/lib/s_stat.h
OBJLINK := --std=c99

export CC TOPDIR OBJDIR BINDIR BIN OBJLINK OBJ

main: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o bin/main

lib: $(LIB)
	$(CC) $(CFLAGS) -c lib/t_stat.c -o obj/t_stat.o

obj/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o obj/main.o

obj/t_stat.o: $(LIB)
	$(CC) $(CFLAGS) -c lib/t_stat.c -o obj/t_stat.o
