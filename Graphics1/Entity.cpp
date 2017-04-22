#include "Level.h"
#include "Entity.h"
#include "Globals.h"
#include <math.h>

Entity::Entity() {
	maxSpeed = DEFAULT_MAXSPEED;
	onGround = false;
	moving = false;
	visAngle = 0.0;
	id = "entity";
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


// Updates the entity
void Entity::update() {
	Vec2D g;
	Level* level = (Level*)state;
	level->getGravityAtPos(pos, &g);
	if (!onGround) {
		vel.addTo(g.multiply(TICKRATE));
	}
	//Rotation
	if (abs(g.getX()) > FLOAT_ZERO || abs(g.getY()) > FLOAT_ZERO) {
		angle = atan2(g.getY(), g.getX()) * RAD_TO_DEG + 90;
	}
	//Max speed checks
	double dX = getVelRelX(angle);
	if (dX > maxSpeed) {
		Vec2D h = Vec2D(-g.getY(), g.getX());
		if (h.magnitudeSquare() > FLOAT_ZERO) {
			h.toUnit();
		} else {
			h.set(cos(DEG_TO_RAD * angle), sin(DEG_TO_RAD * angle));
		}
		h.multiplyBy(dX - maxSpeed);
		vel.subtractFrom(h);
	} else if (dX < -maxSpeed) {
		Vec2D h = Vec2D(-g.getY(), g.getX());
		if (h.magnitudeSquare() > FLOAT_ZERO) {
			h.toUnit();
		} else {
			h.set(cos(DEG_TO_RAD * angle), sin(DEG_TO_RAD * angle));
		}
		h.multiplyBy(dX + maxSpeed);
		vel.subtractFrom(h);
	}
	//Update position
	pos.addTo(vel.multiply(TICKRATE));
	//Track how far the entity has visually rotated
	visAngle = updatedVisAngle(TICKRATE);
	//Increment animation
	currentAnim.addTime(TICKRATE);
}


// Draws the entity
void Entity::draw(double ex) {
	visAngle = updatedVisAngle(ex);
	double f = flip ? -1.0 : 1.0;
	glPushMatrix();
	glTranslated(pos.getX() + vel.getX()*ex, pos.getY() + vel.getY()*ex, 0.0);
	glRotated(visAngle, 0.0, 0.0, 1.0);
	currentAnim.setWidth(abs(currentAnim.getWidth()) * f);
	currentAnim.draw(ex);
#ifdef DEBUG
	//Draw velocity
	glRotated(-visAngle, 0.0, 0.0, 1.0);
	glColor3ub(0, 0, 255);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2d(0.0, 0.0);
	glVertex2d(vel.getX(), vel.getY());
	glEnd();
	//Draw hitbox
	//Note: The laggy appearance of the hitbox is because the game runs at 20hz
	//and is made to appear smoother by extrapolating graphical positions when
	//the game is drawn
	glTranslated(-pos.getX() - vel.getX()*ex, -pos.getY() - vel.getY()*ex, 0.0);
	glColor3ub(onGround ? 0 : 255, 127, 0);
	glLineWidth(2);
	int n;
	Vec2D* vecs = getVertices(&n);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < n; i++) {
		glVertex2d(vecs[i].getX(), vecs[i].getY());
	}
	glEnd();
#endif
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


// Gets the normal vectors needed to check collision
Vec2D* Entity::getNormals(int* numNormals) {
	*numNormals = 2;
	Vec2D* r = new Vec2D[2];
	//Calculate the angle of one of the normals
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
	Vec2D w = Vec2D(cos(DEG_TO_RAD * angle), sin(DEG_TO_RAD * angle));
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
	} else {
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


// Returns if the selectable can be moved
bool Entity::canMove() {
	return true;
}


// Returns if the selectable can be rotated
bool Entity::canResize() {
	return false;
}


// Returns if the selectable can be rotated
bool Entity::canRotate() {
	return true;
}


// Returns whether the entity is currently moving
bool Entity::isMoving() {
	return moving;
}
