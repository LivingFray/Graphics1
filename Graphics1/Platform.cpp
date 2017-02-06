#include "Platform.h"



Platform::Platform() {
	pos = Vec2D(0.0, 0.0);
	width = 0.0;
	height = 0.0;
	angle = 0.0;
}


Platform::~Platform() {
}


// Gets the position of the platform
Vec2D Platform::getPos() {
	return Vec2D();
}


// Sets the position of the platform
void Platform::setPos(Vec2D pos) {
	this->pos = pos;
}

// Sets the width of the platform
void Platform::setWidth(double width) {
	this->width = width;
}


// Sets the height of the platform
void Platform::setHeight(double height) {
	this->height = height;
}


// Gets the width of the platform
double Platform::getWidth() {
	return width;
}


// Gets the height of the platform
double Platform::getHeight() {
	return height;
}

// Sets the angle of the platform
void Platform::setAngle(double angle) {
	this->angle = angle;
}


// Gets the angle of the platform
double Platform::getAngle() {
	return angle;
}
