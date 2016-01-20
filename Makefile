#makefile for Linux
virtustan: virtustan.c virtustan.h world.h proolskript.c readw.c
	gcc -Wno-format-security virtustan.c -o virtustan
	strip virtustan
clean:
	rm virtustan
