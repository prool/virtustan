// prools grafix experiments

// examples from http://steps3d.narod.ru/tutorials/ogl3-tutorial.html
// and from http://blablacode.ru/game-devel/174

#include	<GL/freeglut.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<math.h>
#include	<GL/glext.h>

#include "../proollib/proollib.h"

#define DELTA 0.01f

float global_x;
float global_y;
int text_x, text_y;

void init ()
{
    glClearColor ( 0, 0, 0, 1 );
    glEnable     ( GL_DEPTH_TEST );
    glDepthFunc  ( GL_LEQUAL );
}

#if 0
void test1 ()
{
glBegin(GL_TRIANGLES); glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -2.0f);

glColor3f(0.0f, 1.0f, 0.0f);
glVertex3f( 0.0f,  1.0f, -2.0f);

glColor3f(0.0f, 0.0f, 1.0f);
glVertex3f( 1.0f, -1.0f, -2.0f);

glEnd();
}
#endif


void draw_string(void *font, const char* string) 

{

  while(*string)

    glutStrokeCharacter(font, *string++);

}

void drawText(const char *text, int length, int x, int y)
{
	glMatrixMode(GL_PROJECTION);
	double matrix [16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0,400,0,400,-5,5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x,y);
	for(int i=0; i<length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);

}

void display ()
{float rtri;
float dx, dy;
int i, j;
signed char c;

dx=1.0f-DELTA;
dy=1.0f-DELTA;

//printf("%s\n", ptime());

glClearColor (0.2, 0.0, 0.0, 0.0);
glClear(GL_COLOR_BUFFER_BIT);   ///очистим буфер
 
glColor3f(0.0,1.0,0.0); ///зададим цвет которым будем рисовать

glBegin(GL_POINTS);
glVertex2f(global_x,global_y);   /// точка 
glEnd();

glBegin(GL_LINES);glVertex2f(-dx,-dy);glVertex2f(-dx,+dy);glEnd();
glBegin(GL_LINES);glVertex2f(-dx,-dy);glVertex2f(+dx,-dy);glEnd();
glBegin(GL_LINES);glVertex2f(+dx,+dy);glVertex2f(+dx,-dy);glEnd();
glBegin(GL_LINES);glVertex2f(+dx,+dy);glVertex2f(-dx,+dy);glEnd();

c=0;

for (i=0;i<16;i++)
	{
	for (j=0;j<16;j++)
		{
		drawText(&c,1,text_x+j*8,text_y-i*12);
		c++;
		}
	printf("i=%i\n", i);
	}

glFlush();
glutSwapBuffers(); 
glutPostRedisplay();
#if 0 // работает (рисует треугольник)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glOrtho(-200.0,200.0,-200.0,200.0,-200.0,200.0); ///зададим размеры координатной сетки
    glRotatef(rtri,0.0f,1.0f,1.0f); //вращение вокруг оси oy oz
    glBegin(GL_TRIANGLES);
            glColor3f(1.0f,0.0f,0.0f);      // Красный цвет
                    glVertex3f( -100.0, 0,0); 
            glColor3f(0.0f,1.0f,0.0f);      // Зеленый цвет
                    glVertex3f(0.0,100.0,0); 
            glColor3f(0.0f,0.0f,1.0f);      // Синий цвет
                    glVertex3f( 100.0f,0.0,0); 
    glEnd();
    glutSwapBuffers();
    glutPostRedisplay();
#endif
#if 0
  glClear(GL_COLOR_BUFFER_BIT);


  glColor3f(0.0f, 0.0f, 1.0f);
  glLineWidth(1);

  glBegin(GL_LINES);
    glVertex2f(0, 0.5f);  
    glVertex2f(0, -0.5f);  
  glEnd();

  glFlush();
#endif
#if 0
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glutSwapBuffers ();
#endif

}

void reshape ( int w, int h )
{
glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
#if 0 // я пока мало что в этом понимаю, но эти строки мешают, с ними не работает. prool fool
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
//gluOrtho2D(-1, 1, -1, 1);
glMatrixMode(GL_MODELVIEW);
#endif
    printf("prool: reshape\n");
}

void print_coord(void)
{
printf("point (%f, %f)\n", global_x, global_y);
}

void print_text_coord(void)
{
printf("text (%i, %i)\n", text_x, text_y);
}

void key ( unsigned char key, int x, int y )
{
switch (key)
	{
	case 27:
	case 'q':
	case 'Q': exit(0);
	case ' ': global_x+=DELTA; global_y+=DELTA; print_coord(); break;
	case 'n': global_y+=DELTA; print_coord(); break; // north
	case 's': global_y-=DELTA; print_coord(); break; // south
	case 'w': global_x-=DELTA; print_coord(); break; // south
	case 'e': global_x+=DELTA; print_coord(); break; // south
	case 'N': text_y++; print_text_coord(); break; // text north
	case 'S': text_y--; print_text_coord(); break; // 
	case 'W': text_x--; print_text_coord(); break; // 
	case 'E': text_x++; print_text_coord(); break; // 
	default: printf("key = %i\n", key);
	}
}

int main ( int argc, char * argv [] )
{
global_x=0.0f;
global_y=0;
text_x=10;
text_y=370;
                                // initialize glut
    glutInit            ( &argc, argv );
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize  ( 1200, 600 );

#if 0 // я пока мало что в этом понимаю, но эти строки мешают, с ними не работает. prool fool
                                // prepare context for 3.3
    glutInitContextVersion ( 3, 3 );
    glutInitContextFlags   ( GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG );
    glutInitContextProfile ( GLUT_CORE_PROFILE );
#endif

                                // create window
    glutCreateWindow ( "prool grafix test (q - quit)" );

                                // register handlers
    glutDisplayFunc  ( display );
    glutReshapeFunc  ( reshape );
    glutKeyboardFunc ( key     );

    const char * slVer = (const char *) glGetString ( GL_SHADING_LANGUAGE_VERSION );
    printf ( "GLSL Version: %s\n", slVer );


    glutMainLoop ();

    return 0;
}
