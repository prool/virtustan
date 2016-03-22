// prools grafix experiments

// examples from http://steps3d.narod.ru/tutorials/ogl3-tutorial.html
// and from http://blablacode.ru/game-devel/174

#include	<GL/freeglut.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<math.h>
#include	<GL/glext.h>

#include "FTGL/ftgl.h"

#include "../proollib/proollib.h"

#define MAXSTR 120

#define DELTA 0.010f
#define STEP_X 0.05f
#define STEP_Y 0.10f

#define ftgl_d 0.01f

#define POLE_I 39
#define POLE_J 19

#ifdef MINGW
#define random rand
#endif

// global variables
long time_, timebase;
float global_x;
float global_y;
float qdx=0.047f;
float qdy=0.095f;
float FPS = 0;
int text_x, text_y;
float ftgl_x=-0.99f, ftgl_y=0.93f;
int cursor_i, cursor_j;
int creeper_i=10, creeper_j=10;
int oldx, oldy;
int help_flag=0;
int frame;
char head_str[MAXSTR];

struct  polya
	{
	int room_type; // 0 - notype
	float r;
	float g;
	float b;
	}
	pole [POLE_I][POLE_J];

// functions

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
{int i;
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
	for(i=0; i<length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);

}

char *room_text(int room_type)
{
switch(room_type)
	{
	case 1: return "Вскопано";
	case 0: // typeless ;
	default: return "Typeless room";
	}
}

float room_color(int room_type, int color_no)
{
switch(room_type)
	{
	case 1: if (color_no==1) return 0.0f; else if (color_no==2) return 0.0f; else return 0.0f;
	case 0: // typeless ;
	default: if (color_no==1) return 0.0f; else if (color_no==2) return 0.0f; else return 1.0f;
	}
}

void display ()
{float rtri;
float dx, dy;
int i, j;
signed char c;
float x,y;
float cursor_coord_x, cursor_coord_y;
float quad_x, quad_y;

dx=1.0f-DELTA;
dy=1.0f-DELTA;

//printf("%s\n", ptime());

glClearColor (0.2, 0.0, 0.0, 0.0);
glClear(GL_COLOR_BUFFER_BIT);   ///очистим буфер
 
glColor3f(0.0,1.0,0.0); ///зададим цвет которым будем рисовать

//glBegin(GL_POINTS); glVertex2f(global_x,global_y); glEnd();

if (help_flag)
	{ // help
#if 0 // out ascii table
c=0;
for (i=0;i<16;i++)
	{
	for (j=0;j<16;j++)
		{
		drawText(&c,1,text_x+j*8,text_y-i*12);
		c++;
		}
	//printf("i=%i\n", i);
	}
#endif

#define HELP_TXT "? - exit from help. ` - out FPS. arrows, mouse left button - ... / - till"
drawText(HELP_TXT,strlen(HELP_TXT),text_x,text_y);

	}
else
	{

#if 1
// Код для вычисления кадров в секунду
	frame++;
 
	time_=glutGet(GLUT_ELAPSED_TIME);
	if (time_ - timebase > 1000) {
		FPS=frame*1000.0/(time_-timebase);
		//sprintf(head_str,"FPS:%4.2f", frame*1000.0/(time_-timebase));
		timebase = time_;
		frame = 0;
	}
#endif

#if 0 // вывод текста обычным шрифтом (не ttf)
//strcpy(head_str,"<HEAD>");
drawText(room_text(pole[cursor_i][cursor_j].room_type),
	strlen(room_text(pole[cursor_i][cursor_j].room_type)),10,390);
#endif

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

// kvadrategi
glBegin(GL_QUADS);
for (i=0;i<POLE_I;i++)
	for (j=0;j<POLE_J;j++)
		{
		glColor3f(room_color(pole[i][j].room_type,1),
			room_color(pole[i][j].room_type,2),room_color(pole[i][j].room_type,3));
		quad_x=-dx + (i*STEP_X) + STEP_X/2 - 0.025f;
		quad_y= dy - (j*STEP_Y) - STEP_Y + 0.017f;
		glVertex2f(quad_x, quad_y);
		glVertex2f(quad_x+qdx, quad_y);
		glVertex2f(quad_x+qdx, quad_y-qdy);
		glVertex2f(quad_x, quad_y-qdy);
		}
// creeper
		i=creeper_i; j=creeper_j;
		//pole[i][j].g=0.9f; // крипер не красит!
		glColor3f(1.0f,0.0f,0.0f); // цвет
		quad_x=-dx + (i*STEP_X) + STEP_X/2 - 0.008f;
		quad_y= dy - (j*STEP_Y) - STEP_Y + 0.0f;
		glVertex2f(quad_x, quad_y);
		glVertex2f(quad_x+0.02f, quad_y);
		glVertex2f(quad_x+0.02f, quad_y-0.06f);
		glVertex2f(quad_x, quad_y-0.06f);
glEnd();

// алгоритм крипера
// 1. лезем или стоим на месте
if (random()<(RAND_MAX/90)) // лезем?
	{// лезем
	// 2. куда лезем
if (random()<(RAND_MAX/4))
	{
	creeper_i++; if (creeper_i>=POLE_I) creeper_i--;
	}
else if (random()<(RAND_MAX/3))
	{
	creeper_i--; if (creeper_i<0) creeper_i++;
	}
else if (random()<(RAND_MAX/2))
	{
	creeper_j--; if (creeper_j<0) creeper_j++;
	}
else 	{
	creeper_j++; if (creeper_j>=POLE_J) creeper_j--;
	}
	} // end of лезем

// cursor

cursor_coord_x=-dx + (cursor_i*STEP_X) + STEP_X/2;
cursor_coord_y= dy - (cursor_j*STEP_Y) - STEP_Y - 0.015f;
glBegin(GL_TRIANGLES);
glColor3f(1.0,1.0,0.0); ///зададим цвет которым будем рисовать
glVertex2f(cursor_coord_x,cursor_coord_y);
glVertex2f(cursor_coord_x+0.02f,cursor_coord_y-0.02f);
glVertex2f(cursor_coord_x+0.02f,cursor_coord_y+0.02f);
glEnd();

#if 1 // FTGL
// FTGL from: https://www.linux.org.ru/forum/development/6091539
glRasterPos2f(ftgl_x, ftgl_y);
//FTGLPixmapFont("/usr/share/fonts/truetype/ttf-dejavu/DejaVuSansMono.ttf");
FTGLfont *font = ftglCreatePixmapFont("/usr/share/fonts/truetype/ttf-dejavu/DejaVuSansMono.ttf");
/* If something went wrong, bail out. */
if(!font)
	{
	printf("FTGL font error\n");
    	return;
	}

/* Set the font size and render a small text. */
ftglSetFontFaceSize(font, 22, 22);
ftglRenderFont(font, room_text(pole[cursor_i][cursor_j].room_type) , FTGL_RENDER_ALL);

/* Destroy the font object. */
ftglDestroyFont(font);

glRasterPos2f(1, -4);
glutSwapBuffers();
#endif
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

void print_rgb(void)
{
//printf("RGB = (%f, %f, %f)\n", global_r, global_g, global_b);
}

void print_ftgl_coord(void)
{
printf("FTGL coord (%f, %f)\n", ftgl_x, ftgl_y);
}

void key2 ( int key, int x, int y )
{
switch(key)
	{
	case GLUT_KEY_F1: printf("pressed F1\n"); break;
	case GLUT_KEY_UP: cursor_j--; print_coord(); break; // north
	case GLUT_KEY_DOWN: cursor_j++; print_coord(); break; // south
	case GLUT_KEY_LEFT: cursor_i--; print_coord(); break; // south
	case GLUT_KEY_RIGHT: cursor_i++; print_coord(); break; // south
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
	case 'N': ftgl_y+=ftgl_d; print_ftgl_coord(); break; // text north
	case 'S': ftgl_y-=ftgl_d; print_ftgl_coord(); break; // 
	case 'W': ftgl_x-=ftgl_d; print_ftgl_coord(); break; // 
	case 'E': ftgl_x+=ftgl_d; print_ftgl_coord(); break;
	case '1': glutSwapBuffers(); break; // for testing. proolfool
	case '?': if (help_flag==0) help_flag=1; else help_flag=0; break;
	case '`': sprintf(head_str,"FPS:%4.2f", FPS); break;
	case '/': pole[cursor_i][cursor_j].room_type=1; break; // till
#if 0
	case 'R': global_r+=0.01f; print_rgb(); break;
	case 'r': global_r-=0.01f; print_rgb(); break;
	case 'G': global_g+=0.01f; print_rgb(); break;
	case 'g': global_g-=0.01f; print_rgb(); break;
	case 'B': global_b+=0.01f; print_rgb(); break;
	case 'b': global_b-=0.01f; print_rgb(); break;
#endif
	default: printf("key = %i\n", key);
	}
}

void mouseButton(int button, int state, int x, int y) {
int d, i, j;
 
	if (button == GLUT_LEFT_BUTTON) {
		if (state==GLUT_DOWN) {
			printf("glut left button. x=%4i y=%4i", x, y);
			d=x-oldx; oldx=x;
			//printf(" DX=%i", d);
			i=(x-6)/30;
			printf(" cursor_i=%2i", i);
			cursor_i=i;

			d=y-oldy; oldy=y;
			//printf(" DY=%i", d);
			j=(y-28)/30;
			printf(" cursor_j = %2i\n", j);
			cursor_j=j;
			}
		}
}

int main ( int argc, char * argv [] )
{
int i,j;

global_x=0.0f;
global_y=0;
text_x=10;
text_y=370;
cursor_i=0;
cursor_j=0;
oldx=0;
oldy=0;

for (i=0;i<MAXSTR;i++) head_str[i]='*';
head_str[MAXSTR-1]=0;

// init pole

for (i=0;i<POLE_I;i++)
	for (j=0;j<POLE_J; j++)
		{
		pole[i][j].room_type=0;
		pole[i][j].r=0.0f; //if(random()<(RAND_MAX/100)) pole[i][j].r=1.0f;
		pole[i][j].g=0.0f;
		pole[i][j].b=1.0f;
		}
// initialize glut
glutInit            ( &argc, argv );
glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
glutInitWindowPosition(0,0/*100,100*/);
glutInitWindowSize  ( 1200, 600 );

#if 0 // я пока мало что в этом понимаю, но эти строки мешают, с ними не работает. prool fool
                                // prepare context for 3.3
    glutInitContextVersion ( 3, 3 );
    glutInitContextFlags   ( GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG );
    glutInitContextProfile ( GLUT_CORE_PROFILE );
#endif

                                // create window
    glutCreateWindow ( "Prool Grafix (q - quit, ? - help)" );

                                // register handlers
glutDisplayFunc  ( display );
glutReshapeFunc  ( reshape );
glutKeyboardFunc ( key     );
glutSpecialFunc ( key2     );

glutMouseFunc(mouseButton);

#ifndef MINGW
    const char * slVer = (const char *) glGetString ( GL_SHADING_LANGUAGE_VERSION );
    printf ( "GLSL Version: %s\n", slVer );
#endif

    glutMainLoop ();

    return 0;
}
