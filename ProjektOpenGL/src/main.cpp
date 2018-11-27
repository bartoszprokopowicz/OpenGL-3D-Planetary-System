/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj� osi 
//  uk�adu wsp�rzednych

/*************************************************************************************/


#include <windows.h>
#include <gl/gl.h>
#include <glut.h>
#include <math.h>
#include <iostream>
#include <time.h>
#include "Egg.h"
#include "coords.h"
#include "Camera.h"

#define FPS 1000 / 60

typedef float point3[3];

static GLfloat observer[] = { 0.0, 0.0, 15.0 };

int model = 1;

int WIDTH = 600, HEIGHT = 600;

//Zmienne dla poruszania sie planet
float x, z;
int angle = 0;
int r = 10;

int a = 18;
int b = 15;

float xoff;

// Zmienna przechowuj�ca stan naci�ni�cia klawiszy
// Zmienia warto�� na 1 je�eli klawisz LMB jest naci�ni�ty
// Zmienia warto�� na 2 je�eli klawisz RMB jest naci�ni�ty
int mouseButton = 0;

// Zmienne odpowiedzialne za ruch po okr�gu

Egg egg(10, 5);

Egg egg2(10, 1);

Camera camera;

int frame = 0;
float timer, timerbase = 0;

void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz�tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz�tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz�tek i koniec obrazu osi y

	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x

	glVertex3fv(x_min);
	glVertex3fv(x_max);

	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y

	glVertex3fv(y_min);
	glVertex3fv(y_max);

	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z

	glVertex3fv(z_min);
	glVertex3fv(z_max);

	glEnd();
}

void Ellipse(int _xoff)
{
	float x, z;

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	for (int _angle = 0.0; _angle < 361.0; _angle++)
	{
		x = a * cosf(_angle * (float)M_PI / 180.0f) + xoff;
		z = b * sinf(_angle * (float)M_PI / 180.0f);
		glVertex3f(x, 0.0f, z);
	}
		
	glEnd();
}

/*************************************************************************************/

// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana gdy trzeba
// przerysowa� scen�)



void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej

	camera.Refresh(mouseButton);

	// Narysowanie osi przy pomocy funkcji zdefiniowanej ni�ej
	Axes();
	
	Ellipse(xoff);
	// Narysowanie obiektu jajka
	egg.draw(model);
	
	glTranslatef(x, 0.0, z);

	egg2.draw(model);

	glutSwapBuffers();
}

/*************************************************************************************/

// Funkcja ustalaj�ca stan renderowania



void MyInit(void)
{
	egg.generateCloud();
	egg2.generateCloud();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glShadeModel(GL_SMOOTH);
	float aambb1 = (a*a) - (b * b);
	xoff = sqrt(aambb1);
	// Kolor czyszc�cy (wype�nienia okna) ustawiono na czarny

}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	// Przeliczenie zmiany ilo�ci pikseli w ruchu myszy na stopnie
	camera.Pix2Angle(360.0 / (float)horizontal, 360.0 / (float)vertical);

	// Przej�cie w tryb projekcji
	glMatrixMode(GL_PROJECTION);

	// Czyszczenie macierzy bie��cej
	glLoadIdentity();

	GLfloat aspectRatio = (GLfloat)horizontal / (GLfloat)vertical;

	// Ustawienie parametr�w dla rzutu perspektywicznego
	gluPerspective(70, aspectRatio, 1.0, 300.0);

	glViewport(0, 0, horizontal, vertical);

	// Powr�t do trybu modelu
	glMatrixMode(GL_MODELVIEW);

	// Czyszczenie macierzy bie��cej
	glLoadIdentity();
}

void keys(unsigned char key, int x, int y)
{
	if (key == 'q') model = 1;
	if (key == 'w') model = 2;
	if (key == 'e') model = 3;

	//RenderScene(); // przerysowanie obrazu sceny
}

void MouseButtonState(int btn, int state, int x, int y)
{
	// Zmiana stanu zmiennej okreslaj�cej naci�ni�cie guzika
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		camera.setMousePosition(x, y);	
		mouseButton = 1;
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		camera.setRadius(y);
		mouseButton = 2;
	}
	else
	{
		mouseButton = 0;
	}
}

void idle(int) {
	glutPostRedisplay();
}

void Move() {

	timer = glutGet(GLUT_ELAPSED_TIME);

	float temp1 = a * cosf(angle * (float)M_PI / 180) + xoff;

	if(timer - timerbase > (FPS * (temp1/r) * sqrtf(pow(temp1,2) / pow(r, 2)))) {
		angle += 1;
		angle = angle % 361;
		x = a * cosf(angle * (float)M_PI / 180) + xoff;
		z = b * sinf(angle * (float)M_PI / 180);
		timerbase = timer;
	}

	//glutTimerFunc(FPS, Move, 0);
	glutPostRedisplay();
}

void MousePosition(GLsizei x, GLsizei y)
{
	camera.calculatePosition(x, y);
	camera.calculateRadius(y);

	//glutTimerFunc(FPS, idle, 0);
}





void main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutCreateWindow("Planetary System");

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  Bedzie ona wywo�ywana za ka�dym razem
	// gdy zajdzie potrzba przeryswania okna 

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
	// zazmiany rozmiaru okna      

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutMouseFunc(MouseButtonState);
	glutKeyboardFunc(keys);
	//glutPassiveMotionFunc(MousePosition);
	glutMotionFunc(MousePosition);
	glutIdleFunc(Move);
	//glutTimerFunc(FPS, Move, 0);


	MyInit();
	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/