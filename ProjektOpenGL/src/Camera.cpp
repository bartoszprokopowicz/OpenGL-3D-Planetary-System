#include "Camera.h"

Camera::Camera()
{
	theta = 1.0;
	phi = 1.0;

	position[0] = 0.0;
	position[1] = 0.0;
	position[2] = 15.0;

	mouseButton = 0;

	xMouse_old = 0;
	xMouse_delta = 0;
	yMouse_old = 0;
	yMouse_delta = 0;

	radius_start = 15.0;

	speed = 5;
}

void Camera::Refresh(int button)
{
	switch (button) {
	case 1:
		if (cos(phi) <= 0.0)
			theta -= xMouse_delta * xPix2Angle / 20.0;
		else
			theta += xMouse_delta * xPix2Angle / 20.0;

		phi += yMouse_delta * yPix2Angle / 20.0;
		break;
	case 2:
		radius_start += radius_delta * yPix2Angle;
		break;
	default:
		break;
	}

	theta = fmodf(theta, 2 * M_PI);
	phi = fmodf(phi, 2 * M_PI);

	position[1] = radius_start * sin(phi);

	float y;
	if (cos(phi) >= 0.0)
	{
		position[0] = radius_start * cos(theta) * cos(phi);
		position[2] = radius_start * sin(theta) * cos(phi);
		y = 1.0;
	}
	else
	{
		position[0] = radius_start * cos(theta) * cos(phi);
		position[2] = radius_start * sin(theta) * cos(phi);
		y = -1.0;
	}

	gluLookAt(position[0], position[1], position[2], 0.0, 0.0, 0.0, 0.0, y, 0.0);

}

void Camera::calculateRadius(GLsizei y)
{
	radius_delta = y - radius_old;
	setRadius(y);
}

void Camera::calculatePosition(GLsizei x, GLsizei y)
{
	xMouse_delta = x - xMouse_old;
	yMouse_delta = y - yMouse_old;
	setMousePosition(x, y);
}

void Camera::setMousePosition(int xMouse, int yMouse)
{
	xMouse_old = xMouse;
	yMouse_old = yMouse;
}

void Camera::setRadius(GLfloat radius)
{
	radius_old = radius;
}

void Camera::Pix2Angle(GLfloat _xPix2Angle, GLfloat _yPix2Angle)
{
	xPix2Angle = _xPix2Angle;
	yPix2Angle = _yPix2Angle;
}

int Camera::getDelta()
{
	return xMouse_delta + yMouse_delta;
}

void Camera::setPos(float x, float y, float z)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;

	gluLookAt(position[0], position[1], position[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


Camera::~Camera()
{
}
