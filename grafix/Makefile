all:	gr 3d gr2 gr3

gr3: gr3.c
	gcc -o gr3 gr3.c -lglut -lGL -lGLU ../proollib/proollib.a -lfreetype -L/usr/local/lib /usr/lib/x86_64-linux-gnu/libfreetype.a -I /usr/include/freetype2 -lftgl

gr2: gr2.c
	gcc -o gr2 gr2.c -lglut -lGL -lGLU ../proollib/proollib.a -lfreetype -L/usr/local/lib /usr/lib/x86_64-linux-gnu/libfreetype.a -I /usr/include/freetype2 -lftgl

gr:	gr.c
	gcc -o gr gr.c -lglut -lGL -lGLU ../proollib/proollib.a -lfreetype -L/usr/local/lib /usr/lib/x86_64-linux-gnu/libfreetype.a -I /usr/include/freetype2 -lftgl
#	gcc -o gr gr.c -lglut -lGL -lm
#	gcc -o gr gr.c -lglut -lGL -lGLU ../proollib/proollib.a

3d:	3d.c
	gcc -o 3d 3d.c -lglut -lGL -lGLU -lm ../proollib/proollib.a

clean:
	rm gr
