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

#define DELTA 0.010f
#define STEP_X 0.05f
#define STEP_Y 0.10f

float global_x;
float global_y;
int text_x, text_y;
int cursor_i, cursor_j;

int oldx, oldy;

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
float x,y;
float cursor_coord_x, cursor_coord_y;

dx=1.0f-DELTA;
dy=1.0f-DELTA;

//printf("%s\n", ptime());

glClearColor (0.2, 0.0, 0.0, 0.0);
glClear(GL_COLOR_BUFFER_BIT);   ///очистим буфер
 
glColor3f(0.0,1.0,0.0); ///зададим цвет которым будем рисовать

//glBegin(GL_POINTS); glVertex2f(global_x,global_y); glEnd();

// решётка
// vertikal lines
x=-dx;
#define MAX_X_C 40
glBegin(GL_LINES);
glColor3f(0.0,0.0,1.0);
for (i=0;i<MAX_X_C;i++)
	{
	if ((i==0)||(i==MAX_X_C-1)) glColor3f(1.0,0.0,1.0);
	else glColor3f(0.0,0.0,1.0);
	glVertex2f(x,+dy-0.076);glVertex2f(x,-dy);
	x+=STEP_X;
	}
glEnd();
// horizontal lines
y=-dy;
#define MAX_Y_C 20
glBegin(GL_LINES);
glColor3f(0.0,0.0,1.0);
for (i=0;i<MAX_Y_C;i++)
	{
	if ((i==0)||(i==MAX_Y_C-1)) glColor3f(1.0,0.0,1.0);
	else glColor3f(0.0,0.0,1.0);
	glVertex2f(-dx,y);glVertex2f(+dx-0.030,y);
	y+=STEP_Y;
	}
glEnd();

#if 0 // border
glColor3f(1.0,0.0,0.0); ///зададим цвет которым будем рисовать
glBegin(GL_LINES);glVertex2f(-dx,-dy);glVertex2f(-dx,+dy);glEnd();
glBegin(GL_LINES);glVertex2f(-dx,-dy);glVertex2f(+dx,-dy);glEnd();
glBegin(GL_LINES);glVertex2f(+dx,+dy);glVertex2f(+dx,-dy);glEnd();
glBegin(GL_LINES);glVertex2f(+dx,+dy);glVertex2f(-dx,+dy);glEnd();
#endif

// cursor

cursor_coord_x=-dx + (cursor_i*STEP_X) + STEP_X/2;
cursor_coord_y= dy - (cursor_j*STEP_Y) - STEP_Y - 0.015f;
glBegin(GL_TRIANGLES);
glColor3f(1.0,1.0,0.0); ///зададим цвет которым будем рисовать
glVertex2f(cursor_coord_x,cursor_coord_y);
glVertex2f(cursor_coord_x+0.02f,cursor_coord_y-0.02f);
glVertex2f(cursor_coord_x+0.02f,cursor_coord_y+0.02f);
glEnd();

#if 0 // out ascii table
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
#endif

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

printf("prool: reshape()\n");
}

void print_coord(void)
{
printf("cursor (%i, %i)\n", cursor_i, cursor_j);
}

void print_text_coord(void)
{
printf("text (%i, %i)\n", text_x, text_y);
}

void key2 ( int key, int x, int y )
{
switch(key)
	{
	case GLUT_KEY_F1: printf("pressed F1\n"); break;
	default: printf("pressed unknown special key with code %i\n", key);
	}
}

void key ( unsigned char key, int x, int y )
{
switch (key)
	{
	case 'q': printf("Quit!\n"); exit(0);
	case ' ': global_x+=DELTA; global_y+=DELTA; print_coord(); break;
	case 'n': cursor_j--; print_coord(); break; // north
	case 's': cursor_j++; print_coord(); break; // south
	case 'w': cursor_i--; print_coord(); break; // south
	case 'e': cursor_i++; print_coord(); break; // south
	case 'N': text_y++; print_text_coord(); break; // text north
	case 'S': text_y--; print_text_coord(); break; // 
	case 'W': text_x--; print_text_coord(); break; // 
	case 'E': text_x++; print_text_coord(); break;
	case '1': glutSwapBuffers(); break; // for testing. proolfool
	default: printf("key = %i\n", key);
	}
}

void mouseButton(int button, int state, int x, int y) {
int d, i, j;
 
	if (button == GLUT_LEFT_BUTTON) {
		if (state==GLUT_DOWN) {
			printf("glut left button. state=%i x=%i y=%i", state, x, y);
			d=x-oldx; oldx=x;
			printf(" DX=%i", d);
			i=(x-6)/30;
			printf(" cursor i=%i", i);
			cursor_i=i;

			d=y-oldy; oldy=y;
			printf(" DY=%i", d);
			j=(y-28)/30;
			printf(" cursor j = %i\n", j);
			cursor_j=j;
			}
		}
}

int main ( int argc, char * argv [] )
{
global_x=0.0f;
global_y=0;
text_x=10;
text_y=370;
cursor_i=0;
cursor_j=0;
oldx=0;
oldy=0;
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
glutSpecialFunc ( key2     );

glutMouseFunc(mouseButton);

    const char * slVer = (const char *) glGetString ( GL_SHADING_LANGUAGE_VERSION );
    printf ( "GLSL Version: %s\n", slVer );


    glutMainLoop ();

    return 0;
}
