virtustan: virtustan.c
	gcc virtustan.c -o virtustan -lncurses
	strip virtustan
clean:
	rm virtustan
