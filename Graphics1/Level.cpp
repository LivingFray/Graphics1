#include "Level.h"
#include <GLFW\glfw3.h>
#include "Globals.h"

#include <iostream>
Level::Level() {
}


Level::~Level() {
}


// Updates the level
void Level::update() {
}


// Draws the level
void Level::draw(double ex) {
	for (GravityField* f : gravFields) {
		double w = f->width / 2;
		double h = f->height / 2;
		glPushMatrix();
		glTranslated(f->x, f->y, 0.0);
		//I flipped the whole screen earlier so rotations now act clockwise
		glRotated(-f->rotation, 0, 0, 1);
		glBegin(GL_QUADS);
		glColor3ub(0, 0, 255);
		glVertex2d(-1 * w, -1 * h);
		glColor4ub(0, 0, 0, 0);
		glVertex2d(-1 * w, h);
		glVertex2d(w, h);
		glColor3ub(0, 0, 255);
		glVertex2d(w, -1 * h);
		glEnd();
		glPopMatrix();
	}
}


// Loads a level from the given file
void Level::loadLevel(string filePath) {
	//TEMP TESTING
	GravityField* t = new GravityField();
	t->height = 250;
	t->width = 250;
	t->strength = 10;
	t->x = 200;
	t->y = 200;
	t->rotation = 0;
	gravFields.insert(gravFields.begin(),t);
	t = new GravityField();
	t->height = 250;
	t->width = 250;
	t->strength = 10;
	t->x = 200;
	t->y = 275;
	t->rotation = 90;
	gravFields.insert(gravFields.begin(), t);
	t = new GravityField();
	t->height = 250;
	t->width = 250;
	t->strength = 10;
	t->x = 275;
	t->y = 275;
	t->rotation = 180;
	gravFields.insert(gravFields.begin(), t);
	t = new GravityField();
	t->height = 250;
	t->width = 250;
	t->strength = 10;
	t->x = 275;
	t->y = 200;
	t->rotation = 270;
	gravFields.insert(gravFields.begin(), t);
}


// Calculates the force of gravity applied to an object at a location
void Level::getGravityAtPos(double posX, double posY, double* forceX, double* forceY) {
	*forceX = 0;
	*forceY = 0;
	for (GravityField* f : gravFields) {
		//Translate point to be relative to the field's centre
		double x = posX - f->x;
		double y = posY - f->y;
		//Rotate the point back to be AA with the field (-angle)
		double cTheta = cos(-DEG_TO_RAD * f->rotation); //Slight optimisation as trig is expensive
		double sTheta = sin(-DEG_TO_RAD * f->rotation);
		double xPrime = x * cTheta - y * sTheta;
		double yPrime = y * cTheta + x * sTheta;
		//Calculate field strength at point
		if (xPrime >= -f->width / 2 && xPrime <= f->width / 2
			&& yPrime >= -f->height / 2 && yPrime <= f->height / 2) {
			//Rotate this strength back and add it to force
			*forceX += f->strength * sin(DEG_TO_RAD * f->rotation);
			*forceY += f->strength * cos(DEG_TO_RAD * f->rotation);
		}
	}
	if (*forceX == 0 && *forceY == 0) {
		*forceY = defaultGravity;
	}
}
