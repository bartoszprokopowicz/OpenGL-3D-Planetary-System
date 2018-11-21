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

/*************************************************************************************/

// Funkcja rysuj¹ca osie uk³adu wspó³rzêdnych

Egg egg(10);

int model = 1;

int WIDTH = 600, HEIGHT = 600;

int start_timer = time(NULL), now_timer = 0, frame_count = 0;

static GLfloat theta[] = { 0.0, 0.0, 0.0 }; // trzy k¹ty obrotu



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

	glRotatef(theta[0], 1.0, 0.0, 0.0);

	glRotatef(theta[1], 0.0, 1.0, 0.0);

	glRotatef(theta[2], 0.0, 0.0, 1.0);


	// Narysowanie osi przy pomocy funkcji zdefiniowanej ni¿ej
	Axes();
	//Obni¿enie obiektów o 5 punktów 
	glTranslated(0.0, -5.0, 0.0);
	// Narysowanie obiektu jajka
	egg.draw(model);
	// Przesuniêcie wzglêdem o Y
	
	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania

	glutSwapBuffers();
	// Iloœæ FPS
	frame_count++;
	now_timer = time(NULL);
	if (now_timer - start_timer > 0) {
		std::cout << "FPS: " << frame_count / (now_timer - start_timer) << std::endl;
		frame_count = 0;
		start_timer = now_timer;
	}
}

/*************************************************************************************/

// Funkcja ustalaj¹ca stan renderowania



void MyInit(void)
{
	egg.generateCloud();
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

	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  okreœlaj¹cej proporcjê
	// wymiarów okna 

	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0

		vertical = 1;
	
	WIDTH = horizontal;
	HEIGHT = vertical;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkoœciokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)  

	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji 

	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej            

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wspó³czynnika  proporcji okna
	// Gdy okno nie jest kwadratem wymagane jest okreœlenie tak zwanej
	// przestrzeni ograniczaj¹cej pozwalaj¹cej zachowaæ w³aœciwe
	// proporcje rysowanego obiektu.
	// Do okreslenia przestrzeni ograniczj¹cej s³u¿y funkcja
	// glOrtho(...)            

	if (horizontal <= vertical)

		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);

	else

		glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);

	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu                                    

	glLoadIdentity();
	// Czyszcenie macierzy bie¿¹cej

}

/*************************************************************************************/

// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli

void keys(unsigned char key, int x, int y)
{
	if (key == 'q') model = 1;
	if (key == 'w') model = 2;
	if (key == 'e') model = 3;

	RenderScene(); // przerysowanie obrazu sceny
}


void spinEgg(int)
{

	theta[0] -= 0.5;
	if (theta[0] > 360.0) theta[0] -= 360.0;

	theta[1] -= 0.5;
	if (theta[1] > 360.0) theta[1] -= 360.0;

	theta[2] -= 0.5;
	if (theta[2] > 360.0) theta[2] -= 360.0;

	glutTimerFunc(1000 / FPS, spinEgg, 0);

	glutPostRedisplay(); //odœwie¿enie zawartoœci aktualnego okna
}



void main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutCreateWindow("Jajko w 3D");

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
	// (callback function).  Bedzie ona wywo³ywana za ka¿dym razem
	// gdy zajdzie potrzba przeryswania okna 

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
	// zazmiany rozmiaru okna      

	glutKeyboardFunc(keys);

	//glutIdleFunc(spinEgg);

	glutTimerFunc(1000/FPS, spinEgg, 0);

	MyInit();
	// Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst¹pieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// W³¹czenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/