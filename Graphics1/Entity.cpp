#include "Level.h"
#include "Entity.h"
#include "Globals.h"
#include <math.h>

Entity::Entity() {
	vel = Vec2D(0.0, 0.0);
	maxSpeed = DEFAULT_MAXSPEED;
	onGround = false;
	visAngle = 0.0;
	id = "entity";
	idle = ImageLoader::getImage("error");
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
		h.toUnit();
		h.multiplyBy(dX - maxSpeed);
		vel.subtractFrom(h);
	} else if (dX < -maxSpeed) {
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
	//TODO: Animation
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, idle);
	double f = flip ? -1.0 : 1.0;
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glTranslated(pos.getX() + vel.getX()*ex, pos.getY() + vel.getY()*ex, 0.0);
	glRotated(visAngle, 0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(0.5 * -width, 0.5 * -height);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(0.5 * -width, 0.5 * height);
	glTexCoord2d(f, 1.0);
	glVertex2d(0.5 * width, 0.5 * height);
	glTexCoord2d(f, 0.0);
	glVertex2d(0.5 * width, 0.5 * -height);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
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


// Gets the current velocity of the entity
Vec2D Entity::getVel() {
	return vel;
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


// Returns a DataObject representing the storable object
DataObject Entity::save() {
	DataObject entity = Storable::save();
	entity.add("velX", vel.getX());
	entity.add("velY", vel.getY());
	return entity;
}


// Loads the storable object from the DataObject
void Entity::load(DataObject obj) {
	Storable::load(obj);
	vel.setX(obj.getDouble("velX"));
	vel.setY(obj.getDouble("velY"));
}
