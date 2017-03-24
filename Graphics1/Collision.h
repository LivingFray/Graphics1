#pragma once
#include "Vec2D.h"
#include "Collider.h"

namespace Collision {
	// Projects an object onto a vector
	void project(Collider* c, Vec2D vec, double* min, double* max);
	// Calculates if two colliders are intersecting and provides the vector to move one in if so
	bool intersects(Collider* a, Collider* b, Vec2D* res);
	// Calculates the distance between two colliders along one vector
	double distance(Collider* a, Collider* b, Vec2D n);
	//Performs a rough check to see if the colliders have collided
	bool broadCheck(Collider* a, Collider* b);
}