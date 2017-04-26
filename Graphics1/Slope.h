#pragma once
#include "Platform.h"
class Slope :
	public Platform {
public:
	Slope();
	~Slope();
	// Draws the platform ex seconds after last update
	void draw(double ex);
	// Gets the normal vectors needed to check collision
	Vec2D* getNormals(int * numNormals);
	// Gets the vertices of the bounding box
	Vec2D* getVertices(int * numVertices);
};

