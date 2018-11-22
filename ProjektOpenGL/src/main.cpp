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

#define FPS 60

typedef float point3[3];

static GLfloat observer[] = { 0.0, 0.0, 15.0 };

int model = 1;

int WIDTH = 600, HEIGHT = 600;

// Zmienne przchowuj�ce aktualny k�t obrotu obiektu
// Pocz�tkowa pozycja ma k�t 0 stopni
static GLfloat theta = 1.0;
static GLfloat phi = 1.0;

// Zmienna przechowuj�ca stan naci�ni�cia klawiszy
// Zmienia warto�� na 1 je�eli klawisz LMB jest naci�ni�ty
// Zmienia warto�� na 2 je�eli klawisz RMB jest naci�ni�ty
static int mouseButton = 0;

// Zmienne przechowuj�ce pozycj� myszy oraz r�nic�
// W poztycji myszy w czasie
int xMouse_old = 0;
int xMouse_delta = 0;
int yMouse_old = 0;
int yMouse_delta = 0;

// Zmienne przechowuj�ce warto�� zmiany k�ta
// Wyra�on� w ilo�ci pikseli
GLfloat xPix2Angle;
GLfloat yPix2Angle;

// Promie� od obiektu
GLfloat radius_start = 15.0;
GLfloat radius_old = 0.0;
GLfloat radius_delta = 0.0;

// Zmienne odpowiedzialne za ruch po okr�gu
float x1, z1;
int angle = 0;

Egg egg(10);

Egg egg2(10);

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

/*************************************************************************************/

// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana gdy trzeba
// przerysowa� scen�)



void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej

	// Zmiana stanu zmiennej okreslaj�cej naci�ni�cie guzika
	if (mouseButton == 1)
	{
		// Aktualizacja parametr�w k�towych
		if(cos(phi) <= 0.0)
			theta -= xMouse_delta * xPix2Angle;
		else
			theta += xMouse_delta * xPix2Angle;

		phi += yMouse_delta * yPix2Angle;
	}
	if (mouseButton == 2)
	{
		// Aktualizacja promienia
		radius_start += radius_delta * yPix2Angle * 13.0;
	}

	theta = fmodf(theta, 2 * M_PI); 
	phi = fmodf(phi, 2 * M_PI);

	// Zmiana pozycji obserwatora zgodnie z r�wnaniami
	observer[1] = radius_start * sin(phi);

	float y;
	if (cos(phi) >= 0.0) 
	{
		observer[0] = radius_start * cos(theta) * cos(phi);
		observer[2] = radius_start * sin(theta) * cos(phi);
		y = 1.0;
	}
	else
	{
		observer[0] = radius_start * cos(theta) * cos(phi);
		observer[2] = radius_start * sin(theta) * cos(phi);
		y = -1.0;
	}
	gluLookAt(observer[0], observer[1], observer[2], 0.0, 0.0, 0.0, 0.0, y, 0.0);

	// Narysowanie osi przy pomocy funkcji zdefiniowanej ni�ej
	Axes(); 
	
	// Narysowanie obiektu jajka
	egg.draw(model);
	
	glTranslated(x1, 0.0, z1);

	egg2.draw(model);

	glutSwapBuffers();
}

/*************************************************************************************/

// Funkcja ustalaj�ca stan renderowania



void MyInit(void)
{
	egg.generateCloud();
	egg2.generateCloud();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Kolor czyszc�cy (wype�nienia okna) ustawiono na czarny

}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s�
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.



void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	// Przeliczenie zmiany ilo�ci pikseli w ruchu myszy na stopnie
	xPix2Angle = 2 * M_PI / (float)horizontal;
	yPix2Angle = 2 * M_PI / (float)vertical;

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

/*************************************************************************************/

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli

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
		// Aktualizacja pozycji kursora
		xMouse_old = x;
		yMouse_old = y;
		mouseButton = 1;
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		// Aktualizacja promienia
		radius_old = y;
		mouseButton = 2;
	}
	else
		mouseButton = 0;
}

void MousePosition(GLsizei x, GLsizei y)
{
	// Obliczenie r�nicy w pozycji myszy
	xMouse_delta = x - xMouse_old;
	yMouse_delta = y - yMouse_old;
	radius_delta = y - radius_old;

	// Zapisanie aktualnej pozycji myszy
	xMouse_old = x;
	yMouse_old = y;
	radius_old = y;

	// Od�wie�enie widoku
	//glutPostRedisplay();
}

void Move(int) {

	int r = 10;
	angle += 1;
	angle = angle % 361;
	x1 = r * cos(angle * (float)M_PI / 180);
	z1 = r * sin(angle * (float)M_PI / 180);
	
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, Move, 0);
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
	glutMotionFunc(MousePosition);
	//glutIdleFunc(Move);
	glutTimerFunc(1000/ FPS, Move, 0);


	MyInit();
	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/