#include "Entity.h"
#include "Globals.h"
#include <GLFW\glfw3.h>

Entity::Entity() {
	posX = 0.0;
	posY = 0.0;
	velX = 0.0;
	velY = 0.0;
}


Entity::~Entity() {
}


// Gets the x-coordinate of the entity
double Entity::getX() {
	return posX;
}


// Gets the y-coordinate of the entity
double Entity::getY() {
	return posY;
}


// Sets the x-coordinate of the entity
void Entity::setX(double x) {
	posX = x;
}


// Sets the y-coordinate of the entity
void Entity::setY(double y) {
	posY = y;
}


// Gets the horizontal velocity of the entity
double Entity::getVelX() {
	return velX;
}


// Gets the vertical velocity of the entity
double Entity::getVelY() {
	return velY;
}


// Sets the horizontal velocity of the entity
void Entity::setVelX(double x) {
	velX = x;
}


// Sets the vertical velocity of the entity
void Entity::setVelY(double y) {
	velY = y;
}


// Updates the entity
void Entity::update() {
	posX += velX * TICKRATE;
	posY += velY * TICKRATE;
	//TODO: Gravity
}


// Draws the entity
void Entity::draw() {
	//TODO: Images and stuff
	glColor3ub(255, 0, 0);
	glBegin(GL_QUADS);
	glVertex2d(posX - 5, posY - 5);
	glVertex2d(posX - 5, posY + 5);
	glVertex2d(posX + 5, posY + 5);
	glVertex2d(posX + 5, posY - 5);
	glEnd();
}

/*TODO:
Hitboxes
Collision detection
Gravity
Graphics/Animations
Camera
*/

// Adds the given value to the horizontal velocity
void Entity::addVelX(double x) {
	velX += x;
}


// Adds the given vaue to the vertical velocity
void Entity::addVelY(double y) {
	velY += y;
}


// Adds the value to the horizontal position
void Entity::addPosX(double x) {
	posX += x;
}


// Adds the value to the vertical position
void Entity::addPosY(double y) {
	posY += y;
}
