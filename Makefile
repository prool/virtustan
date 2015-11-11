#makefile for Linux
virtustan: virtustan.c
	gcc -Wno-format-security virtustan.c -o virtustan
	strip virtustan
clean:
	rm virtustan
