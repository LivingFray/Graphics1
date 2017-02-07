#include "Collider.h"



Collider::Collider() {
}


Collider::~Collider() {
}

// Gets the normal vectors needed to check collision
Vec2D* Collider::getNormals(int * numNormals) {
	*numNormals = 0;
	return nullptr;
}


// Gets the vertices of the bounding box
Vec2D* Collider::getVertices(int* numVertices) {
	*numVertices = 0;
	return nullptr;
}


// Called when a collision occurs
void Collider::onCollide(Collider* other) {
}
