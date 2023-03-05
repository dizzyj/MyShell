# This is a very simple makefile 
# 
#CFLAGS=-g -Wall 
CC = gcc
CFLAGS=-g -Wall
OFILES = ush.o expand.o builtin.o
NAME = ush

ush: ush.o expand.o builtin.o
	$(CC) $(CFLAGS) -o $(NAME) $(OFILES)

clean:
	rm ush ush.o expand.o
