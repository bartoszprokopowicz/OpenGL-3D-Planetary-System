#pragma once

#include "glut.h"
#include <math.h>

#define M_PI 3.1415926535897932384626433

class Camera
{
private:

	GLfloat position[3];

	// Zmienne przchowuj�ce aktualny k�t obrotu obiektu
    // Pocz�tkowa pozycja ma k�t 0 stopni
	GLfloat theta = 1.0;
	GLfloat phi = 1.0;

	// Zmienna przechowuj�ca stan naci�ni�cia klawiszy
	// Zmienia warto�� na 1 je�eli klawisz LMB jest naci�ni�ty
	// Zmienia warto�� na 2 je�eli klawisz RMB jest naci�ni�ty
	int mouseButton = 0;

	// Zmienne przechowuj�ce pozycj� myszy oraz r�nic�
	// W poztycji myszy w czasie
	int xMouse_old;
	int xMouse_delta;
	int yMouse_old;
	int yMouse_delta;

	int speed;

	// Zmienne przechowuj�ce warto�� zmiany k�ta
	// Wyra�on� w ilo�ci pikseli
	GLfloat xPix2Angle;
	GLfloat yPix2Angle;

	// Promie� od obiektu
	GLfloat radius_start = 15.0;
	GLfloat radius_old;
	GLfloat radius_delta;
public:
	Camera();
	void Refresh(int button);
	void calculateRadius(GLsizei y);
	void calculatePosition(GLsizei x, GLsizei y);
	void setMousePosition(int xMouse, int yMouse);
	void setRadius(GLfloat radius);
	void Pix2Angle(GLfloat _xPix2Angle, GLfloat _yPix2Angle);
	int getDelta();

	~Camera();
};

