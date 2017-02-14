#include "Platform.h"
#include <math.h>
#include "Globals.h"


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

// Gets the normal vectors needed to check collision
Vec2D* Platform::getNormals(int* numNormals) {
	*numNormals = 2;
	Vec2D* r = new Vec2D[2];
	//Calculate the angle of one of the nromals
	r[0].set(cos(DEG_TO_RAD * angle), sin(DEG_TO_RAD * angle));
	//Rotate the other by 90 degrees to get the second normal
	r[1].set(-r[0].getY(), r[0].getX());
	return r;
}

// Gets the vertices of the bounding box
Vec2D* Platform::getVertices(int* numVertices) {
	*numVertices = 4;
	Vec2D* r = new Vec2D[4];
	//Get vector in 1 direction at angle
	//Transpose it to get other vector
	Vec2D w = Vec2D(cos(angle), sin(angle));
	Vec2D h = Vec2D(-w.getY(), w.getX());
	//Scale to represent size of bounding box
	w.multiplyBy(width * 0.5);
	h.multiplyBy(height * 0.5);
	//Use vectors to get 4 corners
	r[0] = pos.add(w).add(h);
	r[1] = pos.subtract(w).add(h);
	r[2] = pos.subtract(w).subtract(h);
	r[3] = pos.add(w).subtract(h);
	return r;
}

// Called when a collision occurs
void Platform::onCollide(Collider* other) {

}


// Draws the platform ex seconds after last update
void Platform::draw(double ex) {
	glPushMatrix();
	glColor3ub(0, 255, 0);
	glTranslated(pos.getX(), pos.getY(), 0.0);
	glRotated(angle, 0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex2d(-width / 2, -height / 2);
	glVertex2d(-width / 2, height / 2);
	glVertex2d(width / 2, height / 2);
	glVertex2d(width / 2, -height / 2);
	glEnd();
	glPopMatrix();
}
