#include "EntityAI.h"
#include "Level.h"


#define ENTITY_ACCELERATION 0.10

EntityAI::EntityAI() {
}


EntityAI::~EntityAI() {
}


//Updates the entity
void EntityAI::update() {
	Level* l = (Level*) state;
	Entity* player = l->getPlayer();
	if (player != NULL) {
		//Calculate desired direction to travel
		Vec2D move = player->getPos().subtract(pos);
		//Get direction possible to walk in
		Vec2D grav;
		l->getGravityAtPos(pos, &grav);
		//If angle between grav and move is <180 move right, else left
		//If move x grav < 0 => right
		double det = move.getX()*grav.getY() - grav.getX()*move.getY();
		//Rotate the vector through 90 degrees
		move = Vec2D(-grav.getY(), grav.getX());
		//Normalise the value
		if (move.magnitudeSquare() > FLOAT_ZERO) {
			move.toUnit();
		} else {
			move.set(cos(DEG_TO_RAD * angle), sin(DEG_TO_RAD * angle));
		}
		if (det < 0) {
			move.multiplyBy(ENTITY_ACCELERATION);
		} else {
			move.multiplyBy(-ENTITY_ACCELERATION);
		}
		vel.addTo(move);
	}
	//Call inherited update (handles moving)
	Entity::update();
}
