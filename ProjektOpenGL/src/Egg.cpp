#include "Egg.h"



float Egg::randFloat()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

Egg::Egg(int density) : density(density)
{
	parametricTable.resize(density, std::vector<pointsNrgb>(density));
}

std::vector<std::vector<pointsNrgb>> Egg::getparametricTable()
{
	return parametricTable;
}

void Egg::generateCloud()
{
	for (int u = 0; u < density; u++) {
		float _u = u / ((float)density - 1);
		for (int v = 0; v < density; v++) {
			float _v = v / ((float)density - 1);
			//Generacja chmury punktów
			parametricTable[u][v].x = ((-90 * pow(_u, 5)) + (225 * pow(_u, 4)) - (270 * pow(_u, 3)) + (180 * pow(_u, 2)) - (45 * _u)) * (cos(M_PI * _v));
			parametricTable[u][v].y = (160 * pow(_u, 4)) - (320 * pow(_u, 3)) + (160 * pow(_u, 2));
			parametricTable[u][v].z = ((-90 * pow(_u, 5)) + (225 * pow(_u, 4)) - (270 * pow(_u, 3)) + (180 * pow(_u, 2)) - (45 * _u)) * (sin(M_PI * _v));
			//Generowanie losowych kolorów
			parametricTable[u][v].r = randFloat();
			parametricTable[u][v].g = randFloat();
			parametricTable[u][v].b = randFloat();
		}
	}
}

void Egg::draw(int model)
{
	switch (model) {
	case 1:
		glBegin(GL_POINTS);
		for (int u = 0; u < density; u++) {
			for (int v = 0; v < density; v++) {
				glColor3f(0.0f, 0.0f, 0.0f);
				glVertex3f(parametricTable[u][v].x, parametricTable[u][v].y, parametricTable[u][v].z);
			}
		}
		glEnd();
		break;
	case 2:
		glBegin(GL_LINES);
		for (int u = 0; u < density; u++) {
			float _u = (u + 1) % density;
			for (int v = 0; v < density; v++) {
				float _v = (v + 1) % density;
				glColor3f(0.0f, 0.0f, 0.0f);
				// Pierwszy trójk¹t
				glVertex3f(parametricTable[u][v].x, parametricTable[u][v].y, parametricTable[u][v].z);
				glVertex3f(parametricTable[_u][v].x, parametricTable[_u][v].y, parametricTable[_u][v].z);
				glVertex3f(parametricTable[_u][_v].x, parametricTable[_u][_v].y, parametricTable[_u][_v].z);

				// Drugi trójk¹t
				
				glVertex3f(parametricTable[_u][_v].x, parametricTable[_u][_v].y, parametricTable[_u][_v].z);
				glVertex3f(parametricTable[u][_v].x, parametricTable[u][_v].y, parametricTable[u][_v].z);
				glVertex3f(parametricTable[u][v].x, parametricTable[u][v].y, parametricTable[u][v].z);
			}
		}
		glEnd();
		break;
	case 3:
		glBegin(GL_TRIANGLES);
		for (int u = 0; u < density; u++) {
			float _u = (u + 1) % density;
			for (int v = 0; v < density; v++) {
				float _v = (v + 1) % density;
				// Pierwszy trójk¹t
				glColor3f(parametricTable[u][v].r, parametricTable[u][v].g, parametricTable[u][v].b);
				glVertex3f(parametricTable[u][v].x, parametricTable[u][v].y, parametricTable[u][v].z);

				glColor3f(parametricTable[_u][v].r, parametricTable[_u][v].g, parametricTable[_u][v].b);
				glVertex3f(parametricTable[_u][v].x, parametricTable[_u][v].y, parametricTable[_u][v].z);

				glColor3f(parametricTable[_u][_v].r, parametricTable[_u][_v].g, parametricTable[_u][_v].b);
				glVertex3f(parametricTable[_u][_v].x, parametricTable[_u][_v].y, parametricTable[_u][_v].z);

				// Drugi trójk¹t
				glColor3f(parametricTable[_u][_v].r, parametricTable[_u][_v].g, parametricTable[_u][_v].b);
				glVertex3f(parametricTable[_u][_v].x, parametricTable[_u][_v].y, parametricTable[_u][_v].z);

				glColor3f(parametricTable[u][_v].r, parametricTable[u][_v].g, parametricTable[u][_v].b);
				glVertex3f(parametricTable[u][_v].x, parametricTable[u][_v].y, parametricTable[u][_v].z);

				glColor3f(parametricTable[u][v].r, parametricTable[u][v].g, parametricTable[u][v].b);
				glVertex3f(parametricTable[u][v].x, parametricTable[u][v].y, parametricTable[u][v].z);
			}
		}
		glEnd();
		break;
	}	
}


Egg::~Egg()
{
}
