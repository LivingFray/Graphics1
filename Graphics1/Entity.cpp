#include "Level.h"
#include "Entity.h"
#include "Globals.h"
#include <math.h>

#define DEFAULT_MAXSPEED 100.0
#define DEFAULT_ENTITY_WIDTH 10.0
#define DEFAULT_ENTITY_HEIGHT 10.0

Entity::Entity() {
	pos = Vec2D(0.0, 0.0);
	vel = Vec2D(0.0, 0.0);
	maxSpeed = DEFAULT_MAXSPEED;
	onGround = false;
	width = DEFAULT_ENTITY_WIDTH;
	height = DEFAULT_ENTITY_HEIGHT;
	angle = 0.0;
	visAngle = 0.0;
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
	if (!onGround) {
		vel.addTo(g);
	}
	//Rotation
	if (abs(g.getX()) < FLOAT_ZERO && abs(g.getY()) < FLOAT_ZERO) {
		angle = 0; //Floating in no gravity
	}
	else {
		angle = atan2(g.getY(), g.getX()) * RAD_TO_DEG + 90;
	}
	//Max speed checks
	double dX = getVelRelX(angle);
	if (dX > maxSpeed) {
		Vec2D h = Vec2D(-g.getY(), g.getX());
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
	visAngle = updatedVisAngle(TICKRATE);
}


// Draws the entity
void Entity::draw(double ex) {
	visAngle = updatedVisAngle(ex);
	//TODO: Images and stuff
	glPushMatrix();
	glColor3ub(255, 0, 0);
	glTranslated(pos.getX() + vel.getX()*ex, pos.getY() + vel.getY()*ex, 0.0);
	glRotated(visAngle, 0.0, 0.0, 1.0);
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
	vel.addTo(Vec2D(x, 0));
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

// Gets the normal vectors needed to check collision
Vec2D* Entity::getNormals(int* numNormals) {
	*numNormals = 2;
	Vec2D* r = new Vec2D[2];
	//Calculate the angle of one of the nromals
	r[0].set(cos(DEG_TO_RAD * angle), sin(DEG_TO_RAD * angle));
	//Rotate the other by 90 degrees to get the second normal
	r[1].set(-r[0].getY(), r[0].getX());
	return r;
}

// Gets the vertices of the bounding box
Vec2D* Entity::getVertices(int* numVertices) {
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
void Entity::onCollide(Collider* other) {

}


// Sets the width of the entity
void Entity::setWidth(double width) {
	this->width = width;
}


// Gets the width of the entity
double Entity::getWidth() {
	return width;
}


// Sets the height of the entity
void Entity::setHeight(double height) {
	this->height = height;
}


// Gets the height of the entity
double Entity::getHeight() {
	return height;
}


// Gets the current position of the entity
Vec2D Entity::getPos() {
	return pos;
}


// Sets the position of the entity
void Entity::setPos(Vec2D pos) {
	this->pos = pos;
}


// Sets whether the entity is currently on the ground
void Entity::setOnGround(bool onGround) {
	this->onGround = onGround;
}


// Gets if the entity is currently on the ground
bool Entity::getOnGround() {
	return onGround;
}


// Gets the angle the entity seen at
double Entity::getVisAngle() {
	return visAngle;
}


// Sets the angle the entity is seen at
void Entity::setVisAngle(double angle) {
	visAngle = angle;
}


// Updates the visible angle for ex seconds of motion
double Entity::updatedVisAngle(double ex) {
	if (visAngle == angle) {
		return visAngle;
	}
	double a = visAngle;
	double opp = 180 + angle;
	if (opp >= 360) {
		opp -= 360;
	}
	if (a > angle && a <= opp) {
		a -= ex*ENTITY_ROTATE_SPEED;
	}
	else {
		a += ex*ENTITY_ROTATE_SPEED;
	}
	if (a < 0) {
		a += 360;
	}
	if (a >= 360) {
		a -= 360;
	}
	double dif = abs(angle - a);
	if (dif <= ex*ENTITY_ROTATE_SPEED || abs(dif - 360) <= ex*ENTITY_ROTATE_SPEED) {
		a = angle;
	}
	return a;
}

// Gets the current velocity of the entity
Vec2D Entity::getVel() {
	return vel;
}
