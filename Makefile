#makefile for Linux
virtustan: virtustan.c virtustan.h world.h proolskript.c readw.c aux-skripts.c
	gcc -static -g -Wno-format-security virtustan.c -o virtustan
clean:
	rm virtustan
