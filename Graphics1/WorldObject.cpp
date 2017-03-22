#include "WorldObject.h"



WorldObject::WorldObject() {
	pos = Vec2D(0.0, 0.0);
	width = SMALLEST_THICKNESS;
	height = SMALLEST_THICKNESS;
	angle = 0.0;
}


WorldObject::~WorldObject() {
}


// Gets the id of the object
string WorldObject::getId() {
	return id;
}


// Returns the centre of the selectable
Vec2D WorldObject::getPos() {
	return pos;
}


// Sets the position of the selectable
void WorldObject::setPos(Vec2D pos) {
	this->pos = pos;
}


// Gets the angle of the selectable
double WorldObject::getAngle() {
	return angle;
}


// Sets the angle of the selectable
void WorldObject::setAngle(double angle) {
	//Move angle in range
	if (angle<0) {
		angle *= -1;
		angle -= (floor(angle) / 360) * 360;
		angle = 360 - angle;
	}
	if (angle > 360) {
		angle -= (floor(angle) / 360) * 360;
	}
	this->angle = angle;
}


// Gets the width of the selectable
double WorldObject::getWidth() {
	return width;
}


// Sets the width of the selectable
void WorldObject::setWidth(double width) {
	this->width = width > SMALLEST_THICKNESS ? width : SMALLEST_THICKNESS;
}


// Gets the height of the selectable
double WorldObject::getHeight() {
	return height;
}


// Sets the width of the selectable
void WorldObject::setHeight(double height) {
	this->height = height > SMALLEST_THICKNESS ? height : SMALLEST_THICKNESS;
}
