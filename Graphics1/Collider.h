#pragma once
#include "Vec2D.h"
#include "WorldObject.h"
class Collider: 
	virtual public WorldObject {
public:
	Collider();
	~Collider();
	// Gets the normal vectors needed to check collision
	virtual Vec2D* getNormals(int* numNormals);
	// Gets the vertices of the bounding box
	virtual Vec2D* getVertices(int* numVertices);
	// Called when a collision occurs
	virtual void onCollide(Collider* other);
	// Returns whether the object is solid
	virtual bool isSolid();
};

