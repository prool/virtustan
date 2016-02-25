proollib.a:	ptime.o zhopa.o
	ar rc proollib.a *.o
	ranlib proollib.a

ptime.o:	ptime.c proollib.h
	gcc -c ptime.c -o ptime.o
zhopa.o:	zhopa.c proollib.h
	gcc -c zhopa.c -o zhopa.o

clean:
	rm *.o *.a