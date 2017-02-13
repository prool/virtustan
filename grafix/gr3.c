// Langton ant grafix demo
// Prool

#include <GL/gl.h>

#define MAX_X 400
#define MAX_Y 200

int pole [MAX_X][MAX_Y];
float zero_x=-1.0f;
float zero_y= 1.0f;
float delta_x=0.005f;
float delta_y=0.01f;

int langton;
int langton_direct;
int langton_x, langton_y;
int langton_table_x[4] = {0, 1,  0, -1};
int langton_table_y[4] = {1, 0, -1,  0};

void Draw( void ) {
    int i,j;
glClear( GL_COLOR_BUFFER_BIT ); 
glColor3f( 1.0f, 1.0f, 1.0f );
glLineWidth( 1 ); 
#if 0
glBegin( GL_LINES ); 
glVertex2f( 0, 0.5f ); 
glVertex2f( 0, -0.5f );
#endif
#if 0
glBegin(GL_POINTS);
glVertex2f(0,0.5f);
#endif

#if 0 // Langton
// Langton ant
if (langton)
	{
	if (pole[langton_x][langton_y]==0)
		{ // 0 = langton black
		// 90 grad left
		langton_direct++;if (langton_direct==4) langton_direct=0;
		// set color black
		pole[langton_x][langton_y]=1;
		}
	else	{ // !0 = langton white
		// 90 grad right
		langton_direct--;if (langton_direct==-1) langton_direct=3;
		// set color white
		pole[langton_x][langton_y]=0;
		}
	// Langton move
	langton_x+=langton_table_x[langton_direct];
	langton_y+=langton_table_y[langton_direct];

	if (langton_x<0) langton=0;
	if (langton_y<0) langton=0;
	if (langton_x>=MAX_X) langton=0;
	if (langton_y>=MAX_Y) langton=0;
	}
#endif

#if 1 // random
pole[random()%400][random()%200]=1;
#endif

for (i=0;i<MAX_Y;i++)
    for (j=0;j<MAX_X;j++)
        {
            if (pole[i][j])
                {
                glBegin(GL_POINTS);
                glVertex2f(zero_x+delta_x*j, zero_y-delta_y*i);
                }
        }
glEnd();
glFlush();
//glutSwapBuffers();
}

int main( int argc, char *argv[] ) { 
    int i,j;

	langton=1;
	langton_x=50;
	langton_y=50;

    for (i=0; i<MAX_Y; i++)
        for (j=0; j<MAX_X; j++)
            pole [i] [j] =0;

pole[1][1]=1;
pole[100][100]=1;

pole[MAX_X-3][MAX_Y-3]=1;

glutInit( &argc, argv );
glutInitWindowSize( 1200, 650 );
glutInitWindowPosition( 0, 0 );
glutCreateWindow( "GL Demo" );
glutDisplayFunc( Draw );
glutIdleFunc( Draw);
glClearColor( 0, 0, 0.09, 0 );
glutMainLoop();
return 0;
}
