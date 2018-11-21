#pragma once
#include <vector>
#include <math.h>
#include "glut.h"
#include "coords.h"


#define M_PI 3.1415926535897932384626433

class Egg
{
private:
	int density;
	std::vector<std::vector<pointsNrgb>> parametricTable;
	float randFloat();

public:
	
	Egg(int density);
	std::vector<std::vector<pointsNrgb>> getparametricTable();
	void generateCloud();
	void draw(int model);

	~Egg();
};

