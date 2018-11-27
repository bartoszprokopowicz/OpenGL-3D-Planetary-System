#pragma once

#include "glut.h"
#include <math.h>

#define M_PI 3.1415926535897932384626433

class Camera
{
private:

	GLfloat position[3];

	// Zmienne przchowuj¹ce aktualny k¹t obrotu obiektu
    // Pocz¹tkowa pozycja ma k¹t 0 stopni
	GLfloat theta = 1.0;
	GLfloat phi = 1.0;

	// Zmienna przechowuj¹ca stan naciœniêcia klawiszy
	// Zmienia wartoœæ na 1 je¿eli klawisz LMB jest naciœniêty
	// Zmienia wartoœæ na 2 je¿eli klawisz RMB jest naciœniêty
	int mouseButton = 0;

	// Zmienne przechowuj¹ce pozycjê myszy oraz ró¿nicê
	// W poztycji myszy w czasie
	int xMouse_old;
	int xMouse_delta;
	int yMouse_old;
	int yMouse_delta;

	int speed;

	// Zmienne przechowuj¹ce wartoœæ zmiany k¹ta
	// Wyra¿on¹ w iloœci pikseli
	GLfloat xPix2Angle;
	GLfloat yPix2Angle;

	// Promieñ od obiektu
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

