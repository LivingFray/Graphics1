#include "Level.h"
#include <GLFW\glfw3.h>
#include "Globals.h"

Level::Level() {
}


Level::~Level() {
}


// Updates the level
void Level::update() {
	//TODO: Update world

	//Update the entities
	for (Entity* e : entities) {
		e->update();
	}
}


// Draws the level
void Level::draw(double ex) {
	glColor3ub(0, 255, 0);
	glBegin(GL_QUADS);
	glVertex2d(0, 50);
	glVertex2d(0, -200);
	glVertex2d(800, -200);
	glVertex2d(800, 50);
	glEnd();
	//Draw the gravity fields
	for (GravityField* f : gravFields) {
		double w = f->width / 2;
		double h = f->height / 2;
		glPushMatrix();
		glTranslated(f->pos.getX(), f->pos.getY(), 0.0);
		//0 degrees should be down, rather than right
		glRotated(f->rotation, 0, 0, 1);
		glBegin(GL_QUADS);
		glColor3ub(0, 0, 255);
		glVertex2d(-1 * w, -1 * h);
		glVertex2d(w, -1 * h);
		glColor4ub(0, 0, 0, 0);
		glVertex2d(w, h);
		glVertex2d(-1 * w, h);
		glEnd();
		glPopMatrix();
	}
	//Draw the platforms

	//Draw the entities
	for (Entity* e : entities) {
		e->draw(ex);
	}
}


// Loads a level from the given file
void Level::loadLevel(string filePath) {
	//TEMP TESTING
	GravityField* t = new GravityField();
	t->height = 450;
	t->width = 250;
	t->strength = 2;
	t->pos = Vec2D(400, 100);
	t->rotation = 45;
	gravFields.insert(gravFields.begin(), t);
	/*
	t = new GravityField();
	t->height = 50;
	t->width = 50;
	t->strength = 10;
	t->x = 200;
	t->y = 275;
	t->rotation = 90;
	gravFields.insert(gravFields.begin(), t);
	t = new GravityField();
	t->height = 50;
	t->width = 50;
	t->strength = 10;
	t->x = 275;
	t->y = 275;
	t->rotation = 180;
	gravFields.insert(gravFields.begin(), t);
	t = new GravityField();
	t->height = 50;
	t->width = 50;
	t->strength = 10;
	t->x = 275;
	t->y = 200;
	t->rotation = 270;
	gravFields.insert(gravFields.begin(), t);
	*/
}

// Calculates the force of gravity applied to an object at a location
void Level::getGravityAtPos(Vec2D pos, Vec2D* grav) {
	grav->set(0, 0);
	for (GravityField* f : gravFields) {
		//Translate point to be relative to the field's centre
		Vec2D p = pos.subtract(f->pos);
		//Rotate the point back to be AA with the field (-angle)
		double cTheta = cos(-DEG_TO_RAD * f->rotation); //Slight optimisation as trig is expensive
		double sTheta = sin(-DEG_TO_RAD * f->rotation);
		double xPrime = p.getX() * cTheta - p.getY() * sTheta;
		double yPrime = p.getY() * cTheta + p.getX() * sTheta;
		//Calculate field strength at point
		if (xPrime >= -f->width / 2 && xPrime <= f->width / 2
			&& yPrime >= -f->height / 2 && yPrime <= f->height / 2) {
			//Rotate this strength back and add it to force
			//(Forces have been rotated through 90 degrees to make 0 rotation = normal gravity
			grav->addTo(Vec2D(
				f->strength * sin(DEG_TO_RAD * (f->rotation + 90)),
				f->strength * cos(DEG_TO_RAD * (f->rotation + 90))
			));
		}
	}
	if (grav->getX() == 0 && grav->getY() == 0) {
		grav->setY(-defaultGravity);
	}
}
