all:	gr 3d

gr:	gr.c
#	gcc -o gr gr.c -lglut -lGL -lm
	gcc -o gr gr.c -lglut -lGL -lGLU ../proollib/proollib.a

3d:	3d.c
	gcc -o 3d 3d.c -lglut -lGL -lGLU -lm ../proollib/proollib.a

clean:
	rm gr
