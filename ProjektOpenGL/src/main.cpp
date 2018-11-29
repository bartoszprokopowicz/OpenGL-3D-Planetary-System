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
#include "Camera.h"

#define FPS 1000 / 60
#define planetsCount 8

typedef float point3[3];

static GLfloat observer[] = { 0.0, 0.0, 15.0 };

int model = 1;

int WIDTH = 600, HEIGHT = 600;

//Zmienne dla poruszania sie planet
//Sta³a g
const float G = 6.67 * pow(10, -9);
//si³a grawitacji
float Fg[planetsCount];

// Zmienna przechowuj¹ca stan naciœniêcia klawiszy
// Zmienia wartoœæ na 1 je¿eli klawisz LMB jest naciœniêty
// Zmienia wartoœæ na 2 je¿eli klawisz RMB jest naciœniêty
int mouseButton = 0;

// Zmienne odpowiedzialne za ruch po okrêgu

Egg sun(10, 5000000);

Egg planet[planetsCount];

Camera camera;

float timer, timerbase = 0;

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

void Ellipse()
{

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	
	//glVertex3f(x, 0.0f, z);
	
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

	//camera.setPos(planet[2].x, 5.0, planet[2].z);

	camera.Refresh(mouseButton);

	// Narysowanie osi przy pomocy funkcji zdefiniowanej ni¿ej
	Axes();
	
	Ellipse();
	// Narysowanie obiektu jajka
	sun.draw(model);
	for (int i = 0; i < planetsCount; i++) {
		glPushMatrix();
		glTranslatef(planet[i].x, 0.0, planet[i].z);
		planet[i].draw(model);
		glPopMatrix();
	}
	
	
	glutSwapBuffers();
}

/*************************************************************************************/

// Funkcja ustalaj¹ca stan renderowania



void MyInit(void)
{
	sun.generateCloud();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glShadeModel(GL_SMOOTH);

	for (int i = 0; i < planetsCount; i++) {
		planet[i] = Egg(10, 1);
		planet[i].generateCloud();
		planet[i].x = 30 + i*10;
		planet[i].z = 0;
		planet[i].distans = sqrtf((planet[i].x * planet[i].x) + (planet[i].z * planet[i].z));
		Fg[i] = G * (planet[i].weight * sun.weight) / (planet[i].distans * planet[i].distans);
		planet[i].vx = 0;
		planet[i].vz = sqrtf(2 * planet[i].distans * (Fg[i] / planet[i].weight));
	}

	//egg2.x = 60;
	//egg2.z = 0;

	//egg2.distans = sqrtf((egg2.x * egg2.x) + (egg2.z * egg2.z));

	//Fg = G * (egg2.weight * egg.weight) / (egg2.distans * egg2.distans);

	//egg2.vz = sqrtf(2 * egg2.distans * (Fg / egg2.weight));
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	// Przeliczenie zmiany iloœci pikseli w ruchu myszy na stopnie
	camera.Pix2Angle(360.0 / (float)horizontal, 360.0 / (float)vertical);

	// Przejœcie w tryb projekcji
	glMatrixMode(GL_PROJECTION);

	// Czyszczenie macierzy bie¿¹cej
	glLoadIdentity();

	GLfloat aspectRatio = (GLfloat)horizontal / (GLfloat)vertical;

	// Ustawienie parametrów dla rzutu perspektywicznego
	gluPerspective(70, aspectRatio, 1.0, 3000.0);

	glViewport(0, 0, horizontal, vertical);

	// Powrót do trybu modelu
	glMatrixMode(GL_MODELVIEW);

	// Czyszczenie macierzy bie¿¹cej
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
	// Zmiana stanu zmiennej okreslaj¹cej naciœniêcie guzika
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

void Move() {

	timer = glutGet(GLUT_ELAPSED_TIME);

	float timerdiff = timer - timerbase;
	
	for (int i = 0; i < planetsCount; i++) {
		

		planet[i].distans = sqrtf((planet[i].x * planet[i].x) + (planet[i].z * planet[i].z));

		Fg[i] = G * (sun.weight * planet[i].weight) / (planet[i].distans * planet[i].distans);

		planet[i].ax = -(planet[i].x / planet[i].distans) * (Fg[i] / planet[i].weight);
		planet[i].az = -(planet[i].z / planet[i].distans) * (Fg[i] / planet[i].weight);

		planet[i].vx -= -planet[i].ax * timerdiff;
		planet[i].vz -= -planet[i].az * timerdiff;

		planet[i].x += planet[i].vx / 2 * timerdiff;
		planet[i].z += planet[i].vz / 2 * timerdiff;

		
	}

	timerbase = timer;
	//egg2.distans = sqrtf((egg2.x * egg2.x) + (egg2.z * egg2.z));

	//Fg = G * (egg.weight * egg2.weight) / (egg2.distans * egg2.distans);

	//egg2.ax = -(egg2.x / egg2.distans) * (Fg / egg2.weight);
	//egg2.az = -(egg2.z / egg2.distans) * (Fg / egg2.weight);

	//egg2.vx -= -egg2.ax * timerdiff;
	//egg2.vz -= -egg2.az * timerdiff;

	//egg2.x += egg2.vx / 2 * timerdiff;
	//egg2.z += egg2.vz / 2 * timerdiff;
	
	glutPostRedisplay();
}

void MousePosition(GLsizei x, GLsizei y)
{
	camera.calculatePosition(x, y);
	camera.calculateRadius(y);
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
	glutIdleFunc(Move);

	MyInit();
	// Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst¹pieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// W³¹czenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/