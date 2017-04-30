#include "Scenery.h"



Scenery::Scenery() {
	id = "scenery";
}


Scenery::~Scenery() {
}


// Updates the scenery
void Scenery::update() {
}


// Draws the scenery
void Scenery::draw(double ex) {
	glPushMatrix();
	glTranslated(pos.getX(), pos.getY(), 0.0);
	glRotated(angle, 0.0, 0.0, 1.0);
	currentAnim->draw(ex);
	glPopMatrix();
}
