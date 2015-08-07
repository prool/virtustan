# makefile for cygwin
virtustan: virtustan.c
	gcc virtustan.c -o virtustan -liconv
	strip virtustan
clean:
	rm virtustan
