/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj¹ osi 
//  uk³adu wspó³rzednych

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

// Zmienne przchowuj¹ce aktualny k¹t obrotu obiektu
// Pocz¹tkowa pozycja ma k¹t 0 stopni
static GLfloat theta = 1.0;
static GLfloat phi = 1.0;

// Zmienna przechowuj¹ca stan naciœniêcia klawiszy
// Zmienia wartoœæ na 1 je¿eli klawisz LMB jest naciœniêty
// Zmienia wartoœæ na 2 je¿eli klawisz RMB jest naciœniêty
static int mouseButton = 0;

// Zmienne przechowuj¹ce pozycjê myszy oraz ró¿nicê
// W poztycji myszy w czasie
int xMouse_old = 0;
int xMouse_delta = 0;
int yMouse_old = 0;
int yMouse_delta = 0;

// Zmienne przechowuj¹ce wartoœæ zmiany k¹ta
// Wyra¿on¹ w iloœci pikseli
GLfloat xPix2Angle;
GLfloat yPix2Angle;

// Promieñ od obiektu
GLfloat radius_start = 15.0;
GLfloat radius_old = 0.0;
GLfloat radius_delta = 0.0;

// Zmienne odpowiedzialne za ruch po okrêgu
float x1, z1;
int angle = 0;

Egg egg(10);

Egg egg2(10);

void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz¹tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz¹tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz¹tek i koniec obrazu osi y

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

// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana gdy trzeba
// przerysowaæ scenê)



void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej

	// Zmiana stanu zmiennej okreslaj¹cej naciœniêcie guzika
	if (mouseButton == 1)
	{
		// Aktualizacja parametrów k¹towych
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

	// Zmiana pozycji obserwatora zgodnie z równaniami
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

	// Narysowanie osi przy pomocy funkcji zdefiniowanej ni¿ej
	Axes(); 
	
	// Narysowanie obiektu jajka
	egg.draw(model);
	
	glTranslated(x1, 0.0, z1);

	egg2.draw(model);

	glutSwapBuffers();
}

/*************************************************************************************/

// Funkcja ustalaj¹ca stan renderowania



void MyInit(void)
{
	egg.generateCloud();
	egg2.generateCloud();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Kolor czyszc¹cy (wype³nienia okna) ustawiono na czarny

}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.



void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	// Przeliczenie zmiany iloœci pikseli w ruchu myszy na stopnie
	xPix2Angle = 2 * M_PI / (float)horizontal;
	yPix2Angle = 2 * M_PI / (float)vertical;

	// Przejœcie w tryb projekcji
	glMatrixMode(GL_PROJECTION);

	// Czyszczenie macierzy bie¿¹cej
	glLoadIdentity();

	GLfloat aspectRatio = (GLfloat)horizontal / (GLfloat)vertical;

	// Ustawienie parametrów dla rzutu perspektywicznego
	gluPerspective(70, aspectRatio, 1.0, 300.0);

	glViewport(0, 0, horizontal, vertical);

	// Powrót do trybu modelu
	glMatrixMode(GL_MODELVIEW);

	// Czyszczenie macierzy bie¿¹cej
	glLoadIdentity();
}

/*************************************************************************************/

// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli

void keys(unsigned char key, int x, int y)
{
	if (key == 'q') model = 1;
	if (key == 'w') model = 2;
	if (key == 'e') model = 3;

	//RenderScene(); // przerysowanie obrazu sceny
}

void MouseButtonState(int btn, int state, int x, int y)
{

	// Zmiana stanu zmiennej okreslaj¹cej naciœniêcie guzika
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
	// Obliczenie ró¿nicy w pozycji myszy
	xMouse_delta = x - xMouse_old;
	yMouse_delta = y - yMouse_old;
	radius_delta = y - radius_old;

	// Zapisanie aktualnej pozycji myszy
	xMouse_old = x;
	yMouse_old = y;
	radius_old = y;

	// Odœwie¿enie widoku
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
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
	// (callback function).  Bedzie ona wywo³ywana za ka¿dym razem
	// gdy zajdzie potrzba przeryswania okna 

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
	// zazmiany rozmiaru okna      

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutMouseFunc(MouseButtonState);
	glutKeyboardFunc(keys);
	glutMotionFunc(MousePosition);
	//glutIdleFunc(Move);
	glutTimerFunc(1000/ FPS, Move, 0);


	MyInit();
	// Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst¹pieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// W³¹czenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/