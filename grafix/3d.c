// prools grafix experiments
// example from http://grafika.me/node/131

#include	<GL/freeglut.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<math.h>
#include	<GL/glext.h>

#include "../proollib/proollib.h"

// угол поворота камеры
float angle=0.0;
// координаты вектора направления движения камеры
float lx=0.0f,lz=-1.0f;
// XZ позиция камеры
float x=0.0f,z=5.0f;
// высота и ширина окна
int h,w;

void *font = GLUT_STROKE_ROMAN;

// переменные для вычисления количества кадров в секунду
int frame;
long time_, timebase;
char s[50];

void renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {
 
	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void renderStrokeFontString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {  
 
	char *c;
	glPushMatrix();
	glTranslatef(x, y,z);
	glScalef(0.002f, 0.002f, 0.002f);
	for (c=string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}
	glPopMatrix();
}
 
void restorePerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	//восстановить предыдущую матрицу проекции
	glPopMatrix();
	//вернуться в режим модели
	glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection() {
	//выбрать режим проекции
	glMatrixMode(GL_PROJECTION);
	//Сохраняем предыдущую матрицу, которая содерж
	//параметры перспективной проекции
	glPushMatrix();
	//обнуляем матрицу
	glLoadIdentity();
	//устанавливаем 2D ортогональную проекцию
	gluOrtho2D(0, w, h, 0);
	//выбираем режим обзора модели
	glMatrixMode(GL_MODELVIEW);
}

void changeSize(int ww, int hh) {
	h = hh;
	w = ww;
	// предотвращение деления на ноль
	if (h == 0)
		h = 1;
	float ratio =  w * 1.0 / h;
	// используем матрицу проекции
	glMatrixMode(GL_PROJECTION);
	// обнуляем матрицу
	glLoadIdentity();
	// установить параметры вьюпорта
	glViewport(0, 0, w, h);
	// установить корректную перспективу
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	// вернуться к матрице проекции
	glMatrixMode(GL_MODELVIEW);
}
 
void drawSnowMan() {
 
	glColor3f(1.0f, 1.0f, 1.0f);
 
// тело снеговика
	glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f,20,20);
 
// голова снеговика
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f,20,20);
// глаза снеговика
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();
// нос снеговика
	glColor3f(1.0f, 0.5f , 0.5f);
	glRotatef(0.0f,1.0f, 0.0f, 0.0f);
	glutSolidCone(0.08f,0.5f,10,2);
}

void renderScene(void) {
	// Очистка буфера цвета и глубины.
	glClearColor (0.0, 0.0, 0.2, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// обнулить трансформацию
	glLoadIdentity();
	// установить камеру
	gluLookAt(   x, 1.0f,     z,
		  x+lx, 1.0f,  z+lz,
		  0.0f, 1.0f,  0.0f );
        // нарисуем "землю"
	//glColor3f(0.9f, 0.9f, 0.9f);
	glColor3f(0.0f, 0.9f, 0.0f);
	glBegin(GL_QUADS); // полигон с координатами
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();
#if 0
        // Нарисуем 64 снеговика
	for(int i = -4; i < 4; i++)
		for(int j=-4; j < 4; j++) {
			glPushMatrix();
			glTranslatef(i*5.0, 0, j * 5.0);
			drawSnowMan();
			glPopMatrix();
		}
#else
        // Нарисуем 1 снеговик
	int i = -4;
		int j=-4;
			glPushMatrix();
			glTranslatef(i*5.0, 0, j * 5.0);
			drawSnowMan();
			renderStrokeFontString(0.0f, 0.5f, 0.0f, (void *)font ,"SnowMan");
			glPopMatrix();
#endif
// Код для вычисления кадров в секунду
	frame++;
 
	time_=glutGet(GLUT_ELAPSED_TIME);
	if (time_ - timebase > 1000) {
		sprintf(s,"FPS:%4.2f",
			frame*1000.0/(time_-timebase));
		timebase = time_;
		frame = 0;
	}
	//Код для отображения строки (кадров в секунду) с растровых шрифтов
	setOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(5,30,0,GLUT_BITMAP_HELVETICA_18,s);
	glPopMatrix();
 
	restorePerspectiveProjection();
	glutSwapBuffers();
}

void processSpecialKeys(int key, int xx, int yy) {
	float fraction = 0.1f;
	switch (key) {
		case GLUT_KEY_LEFT :
			angle -= 0.01f;
			printf("x=%f z=%f angle=%f rad\n", x, z, angle);
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_RIGHT :
			angle += 0.01f;
			printf("x=%f z=%f angle=%f rad\n", x, z, angle);
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_UP :
			x += lx * fraction;
			z += lz * fraction;
			printf("x=%f z=%f angle=%f rad\n", x, z, angle);
			break;
		case GLUT_KEY_DOWN :
			x -= lx * fraction;
			z -= lz * fraction;
			printf("x=%f z=%f angle=%f rad\n", x, z, angle);
			break;
	}
}

void print_help()
{
printf("Help:\n\
? - help\n\
q - quit\n\
arrows - move and rotate\n\
");
}

void processNormalKeys(unsigned char key, int x, int y) {
	switch(key)
		{
		case 'q': exit(0);
		case '?': print_help(); break;
		}
}

int main(int argc, char **argv) {
	// Инициализация и создание окна
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0,0/*100,100*/);
	glutInitWindowSize(1200,600/*400,400*/);
	glutCreateWindow("Prool's 3D test. q - quit");
 
	// Регистрация
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
 
	// Инициализация OpenGL функции теста
	glEnable(GL_DEPTH_TEST);

	printf("3D test. Пруль\nq - quit ? - help");
	printf("x=%f z=%f angle=%f rad\n", x, z, angle);
 
	// Основной цикл GLUT
	glutMainLoop();
 
	return 1;
}
