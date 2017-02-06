#pragma once
#include "Vec2D.h"
class Collider {
public:
	Collider();
	~Collider();
	// Gets the normal vectors needed to check collision
	Vec2D* getNormals(int* numNormals);
	// Gets the vertices of the bounding box
	Vec2D* getVertices(int* numVertices);
};

