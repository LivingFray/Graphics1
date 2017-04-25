#include "EntityAI.h"
#include "Level.h"


#define ENTITY_ACCELERATION 0.10
#define ENTITY_FOLLOW_RANGE 6.0

EntityAI::EntityAI() {
	id = "AI";
	maxSpeed = DEFAULT_MAXSPEED * 0.75;
}


EntityAI::~EntityAI() {
}


//Updates the entity
void EntityAI::update() {
	Level* l = (Level*) state;
	Entity* player = l->getPlayer();
	if (player != NULL) {
		//If the player is in range
		if (player->getPos().subtract(pos).magnitudeSquare() < ENTITY_FOLLOW_RANGE * ENTITY_FOLLOW_RANGE) {
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
				flip = false;
			} else {
				move.multiplyBy(-ENTITY_ACCELERATION);
				flip = true;
			}
			vel.addTo(move);
			moving = true;
		} else {
			moving = false;
		}
	}
	//Call inherited update (handles moving)
	Entity::update();
}
