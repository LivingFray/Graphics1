#include "Entity.h"
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
void Entity::setVelX(double velX) {
	this->velX = velX;
}


// Sets the vertical velocity of the entity
void Entity::setVelY(double velY) {
	this->velY = velY;
}


// Updates the entity
void Entity::update() {
	posX += velX;
	posY += velY;
	//TODO: Gravity
}


// Draws the entity
void Entity::draw() {
	//TODO: Images and stuff
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