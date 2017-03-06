#include "Platform.h"
#include <math.h>
#include "Globals.h"


Platform::Platform() {
	pos = Vec2D(0.0, 0.0);
	width = 0.0;
	height = 0.0;
	angle = 0.0;
	id = "platform";
}


Platform::~Platform() {
}


// Gets the position of the platform
Vec2D Platform::getPos() {
	return pos;
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


// Called when the selectable is moved
bool Platform::onMove(double dX, double dY) {
	pos.addTo(Vec2D(dX, dY));
	return true;
}


// Called when the selectable is resized
bool Platform::onResize(double dX, double dY) {
	if (width + dX <= SMALLEST_THICKNESS) {
		return false;
	}
	if (height + dY <= SMALLEST_THICKNESS) {
		return false;
	}
	width += dX;
	height += dY;
	return true;
}


// Called when the selectable is rotated
bool Platform::onRotate(double dAngle) {
	angle += dAngle;
	if (angle > 360) {
		angle -= 360;
	}
	if (angle < 0) {
		angle += 360;
	}
	return true;
}


// Returns if the selectable can be moved
bool Platform::canMove() {
	return true;
}


// Returns if the selectable can be rotated
bool Platform::canResize() {
	return true;
}


// Returns if the selectable can be rotated
bool Platform::canRotate() {
	return true;
}


// Returns if the selectable is selected
bool Platform::isInBoundingBox(double x, double y) {
	//Translate point to be relative to the BB's centre
	Vec2D p = Vec2D(x, y).subtract(pos);
	//Rotate the point back to be AA with the BB (-angle)
	double cTheta = cos(-DEG_TO_RAD * angle);
	double sTheta = sin(-DEG_TO_RAD * angle);
	double xPrime = p.getX() * cTheta - p.getY() * sTheta;
	double yPrime = p.getY() * cTheta + p.getX() * sTheta;
	//Calculate field strength at point
	return xPrime >= -width / 2 && xPrime <= width / 2
		&& yPrime >= -height / 2 && yPrime <= height / 2;
}


// Returns a DataObject representing the storable object
DataObject* Platform::save() {
	DataObject* platform = new DataObject();
	double* x = new double(pos.getX());
	double* y = new double(pos.getY());
	platform->add("id", STRING, &id);
	platform->add("x", DOUBLE, x);
	platform->add("y", DOUBLE, y);
	platform->add("width", DOUBLE, &width);
	platform->add("height", DOUBLE, &height);
	platform->add("angle", DOUBLE, &angle);
	return platform;
}


// Loads the storable object from the DataObject
void Platform::load(DataObject* obj) {
	DATATYPE type;
	void* data;
	//Default values
	double x = 0.0, y = 0.0;
	double w = 0.0, h = 0.0;
	double a = 0.0;
	//Load in values where possible
	data = obj->get("x", type);
	if (type == DOUBLE) {
		x = *(double*)data;
	}
	data = obj->get("y", type);
	if (type == DOUBLE) {
		y = *(double*)data;
	}
	data = obj->get("width", type);
	if (type == DOUBLE) {
		w = *(double*)data;
	}
	data = obj->get("height", type);
	if (type == DOUBLE) {
		h = *(double*)data;
	}
	data = obj->get("angle", type);
	if (type == DOUBLE) {
		a = *(double*)data;
	}
	pos = Vec2D(x, y);
	width = w>0 ? w : 0;
	height = h>0 ? h : 0;
	angle = a;
}
