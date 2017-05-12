#include "WorldObject.h"



WorldObject::WorldObject() {
	pos = Vec2D(0.0, 0.0);
	vel = Vec2D(0.0, 0.0);
	width = SMALLEST_THICKNESS;
	height = SMALLEST_THICKNESS;
	angle = 0.0;
	id = "object";
}

WorldObject::WorldObject(const WorldObject& other) {
	pos = other.pos;
	vel = other.vel;
	id = other.id;
	width = other.width;
	height = other.height;
	angle = other.angle;
	editorMode = other.editorMode;
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


// Gets the horizontal velocity of the entity
double WorldObject::getVelX() {
	return vel.getX();
}


// Gets the vertical velocity of the entity
double WorldObject::getVelY() {
	return vel.getY();
}


// Sets the horizontal velocity of the entity
void WorldObject::setVelX(double x) {
	vel.setX(x);
}


// Sets the vertical velocity of the entity
void WorldObject::setVelY(double y) {
	vel.setY(y);
}


// Gets the horizontal (relative to the rotation) velocity
double WorldObject::getVelRelX(double theta) {
	double cTheta = cos(-DEG_TO_RAD * theta);
	double sTheta = sin(-DEG_TO_RAD * theta);
	return vel.getX() * cTheta - vel.getY() * sTheta;
}


// Gets the vertical (relative to the rotation) velocity
double WorldObject::getVelRelY(double theta) {
	double cTheta = cos(-DEG_TO_RAD * theta);
	double sTheta = sin(-DEG_TO_RAD * theta);
	return vel.getY() * cTheta + vel.getX() * sTheta;
}


// Gets the current velocity of the entity
Vec2D WorldObject::getVel() {
	return vel;
}


// Gets the current velocity of the entity
void WorldObject::setVel(Vec2D vel) {
	this->vel = vel;
}


// Returns if the world object can be moved
bool WorldObject::canMove() {
	return true;
}


// Returns if the world object can be rotated
bool WorldObject::canResize() {
	return true;
}


// Returns if the world object can be rotated
bool WorldObject::canRotate() {
	return true;
}


// Called when damage is inflicted on the object
void WorldObject::onDamage(Damage d) {
}


// Updates the object
void WorldObject::update() {
}


// Puts the object in editor mode to help placement
void WorldObject::setEditor() {
	editorMode = true;
}
