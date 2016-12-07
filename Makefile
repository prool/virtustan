#makefile for Linux
#CC = gcc
CC = clang
virtustan: virtustan.c virtustan.h world.h proolskript.c readw.c aux-skripts.c roomtypes.h
	$(CC) -g -Wno-format-security virtustan.c -o virtustan
clean:
	rm virtustan
