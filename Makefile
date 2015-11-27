#makefile for Linux
virtustan: virtustan.c world.h
	gcc -Wno-format-security virtustan.c -o virtustan
	strip virtustan
clean:
	rm virtustan
