#include "Selectable.h"



Selectable::Selectable() {
}


Selectable::~Selectable() {
}


// Called when the selectable is moved
void Selectable::onMove(double dX, double dY) {
}


// Called when the selectable is resized
void Selectable::onResize(double dX, double dY) {
}


// Called when the selectable is rotated
void Selectable::onRotate(double dAngle) {
}


// Returns if the selectable can be moved
bool Selectable::canMove() {
	return true;
}


// Returns if the selectable can be rotated
bool Selectable::canResize() {
	return true;
}


// Returns if the selectable can be rotated
bool Selectable::canRotate() {
	return true;
}


// Returns if the selectable is selected
bool Selectable::isInBoundingBox(double x, double y) {
	return false;
}


// Returns the centre of the selectable
Vec2D Selectable::getPos() {
	return Vec2D(0, 0);
}


// Sets the position of the selectable
void Selectable::setPos(Vec2D pos) {
}
