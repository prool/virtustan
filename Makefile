#makefile for Linux
virtustan: virtustan.c virtustan.h world.h proolskript.c readw.c aux-skripts.c roomtypes.h
	gcc -g -Wno-format-security virtustan.c -o virtustan
clean:
	rm virtustan
