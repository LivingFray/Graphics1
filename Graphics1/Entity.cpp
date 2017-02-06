#include "Entity.h"
#include "Globals.h"
#include <GLFW\glfw3.h>
#include <math.h>

#define DEFAULT_MAXSPEED 100.0

Entity::Entity() {
	pos = Vec2D(0.0, 0.0);
	vel = Vec2D(0.0, 0.0);
	maxSpeed = DEFAULT_MAXSPEED;
}


Entity::~Entity() {
}


// Gets the x-coordinate of the entity
double Entity::getX() {
	return pos.getX();
}


// Gets the y-coordinate of the entity
double Entity::getY() {
	return pos.getY();
}


// Sets the x-coordinate of the entity
void Entity::setX(double x) {
	pos.setX(x);
}


// Sets the y-coordinate of the entity
void Entity::setY(double y) {
	pos.setY(y);
}


// Gets the horizontal velocity of the entity
double Entity::getVelX() {
	return vel.getX();
}


// Gets the vertical velocity of the entity
double Entity::getVelY() {
	return vel.getY();
}


// Sets the horizontal velocity of the entity
void Entity::setVelX(double x) {
	vel.setX(x);
}


// Sets the vertical velocity of the entity
void Entity::setVelY(double y) {
	vel.setY(y);
}


// Updates the entity
void Entity::update() {
	Vec2D g;
	level->getGravityAtPos(pos, &g);
	vel.addTo(g);
	//TODO: Max speed checks
	double dX = getVelRelX(angle);
	if (dX > maxSpeed) {
		Vec2D h = Vec2D(-g.getY(),g.getX());
		h.toUnit();
		h.multiplyBy(dX - maxSpeed);
		vel.subtractFrom(h);
	}
	else if (dX < -maxSpeed) {
		Vec2D h = Vec2D(-g.getY(), g.getX());
		h.toUnit();
		h.multiplyBy(dX + maxSpeed);
		vel.subtractFrom(h);
	}
	pos.addTo(vel.multiply(TICKRATE));
}


// Draws the entity
void Entity::draw(double ex) {
	//TODO: Images and stuff
	glPushMatrix();
	glColor3ub(255, 0, 0);
	glTranslated(pos.getX(), pos.getY(), 0.0);
	glRotated(angle, 0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex2d(-5, -5);
	glVertex2d(-5, +5);
	glVertex2d(+5, +5);
	glVertex2d(+5, -5);
	glEnd();
	glPopMatrix();
}

// Adds the given value to the horizontal velocity
void Entity::addVelX(double x) {
	vel.addTo(Vec2D(x,0));
}


// Adds the given vaue to the vertical velocity
void Entity::addVelY(double y) {
	vel.addTo(Vec2D(0, y));
}


// Adds the value to the horizontal position
void Entity::addPosX(double x) {
	pos.addTo(Vec2D(x, 0));
}


// Adds the value to the vertical position
void Entity::addPosY(double y) {
	pos.addTo(Vec2D(0, y));
}


// Sets the entity's max speed
void Entity::setMaxSpeed(double speed) {
	maxSpeed = speed;
}


// Gets the entity's max speed
double Entity::getMaxSpeed() {
	return maxSpeed;
}


// Gets the horizontal (relative to the rotation) velocity
double Entity::getVelRelX(double theta) {
	double cTheta = cos(-DEG_TO_RAD * theta);
	double sTheta = sin(-DEG_TO_RAD * theta);
	return vel.getX() * cTheta - vel.getY() * sTheta;
}


// Gets the vertical (relative to the rotation) velocity
double Entity::getVelRelY(double theta) {
	double cTheta = cos(-DEG_TO_RAD * theta);
	double sTheta = sin(-DEG_TO_RAD * theta);
	return vel.getY() * cTheta + vel.getX() * sTheta;
}


// Sets the level in which the entity exists
void Entity::setLevel(Level* level) {
	this->level = level;
}


// Gets the level in which the entity exists
Level* Entity::getLevel() {
	return level;
}


// Gets the angle of the entity
double Entity::getAngle() {
	return angle;
}


// Sets the angle of the entity
void Entity::setAngle(double angle) {
	this->angle = angle;
}
