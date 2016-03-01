// prools grafix experiments

// examples from http://steps3d.narod.ru/tutorials/ogl3-tutorial.html
// and from http://blablacode.ru/game-devel/174

#include	<GL/freeglut.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<math.h>
#include	<GL/glext.h>

float global_x;

float global_f;

void init ()
{
    glClearColor ( 0, 0, 0, 1 );
    glEnable     ( GL_DEPTH_TEST );
    glDepthFunc  ( GL_LEQUAL );
}

void test1 ()
{
glBegin(GL_TRIANGLES); glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -2.0f);

glColor3f(0.0f, 1.0f, 0.0f);
glVertex3f( 0.0f,  1.0f, -2.0f);

glColor3f(0.0f, 0.0f, 1.0f);
glVertex3f( 1.0f, -1.0f, -2.0f);

glEnd();
}

void display ()
{float rtri;
#if 1 // работает
   glClear(GL_COLOR_BUFFER_BIT);   ///очистим буфер
 
    glColor3f(0.0,1.0,0.0); ///зададим цвет которым будем рисовать

	glBegin(GL_POINTS);
    	glVertex2f(0,global_f);   /// точка 
	glEnd();

#if 1
    glBegin(GL_LINES);
    glVertex2f(0,0);   ///первая точка оси
    glVertex2f(0,global_x);   ///вторая точка оси
    glEnd();
#endif

    glFlush();
    glutSwapBuffers(); 
    glutPostRedisplay();
#endif
#if 0 // работает (рисует треугольник)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glOrtho(-200.0,200.0,-200.0,200.0,-200.0,200.0); ///зададим размеры координатной сетки
    glRotatef(rtri,0.0f,1.0f,1.0f); //вращение вокруг оси oy oz
    glBegin(GL_TRIANGLES);
            glColor3f(1.0f,0.0f,0.0f);      // Красный цвет
                    glVertex3f( -100.0, 0,0); 
            glColor3f(0.0f,1.0f,0.0f);      // Зеленный цвет
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
#if 0
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluOrtho2D(-1, 1, -1, 1);
  glMatrixMode(GL_MODELVIEW);
#endif
    printf("prool: reshape\n");
}

void key ( unsigned char key, int x, int y )
{
	if (key==' ') {global_x+=0.1f; global_f+=0.1f; }
    else if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
        exit ( 0 );
}

int main ( int argc, char * argv [] )
{
global_x=0;

global_f=0.1f;
                                // initialize glut
    glutInit            ( &argc, argv );
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize  ( 500, 500 );

#if 0
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
