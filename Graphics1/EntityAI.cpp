#include "EntityAI.h"
#include "Level.h"


#define ENTITY_ACCELERATION 0.10
#define ENTITY_FOLLOW_RANGE 12.0
#define ENTITY_SLOWEST_SPEED 1.00

EntityAI::EntityAI() {
	id = "AI";
	maxSpeed = DEFAULT_ENTITY_MAXSPEED * 0.75;
}


EntityAI::~EntityAI() {
}


//Updates the entity
void EntityAI::update() {
	Level* l = (Level*) state;
	Entity* player = l->getPlayer();
	if (player != NULL) {
		//If the player is in range
		double distance = player->getPos().subtract(pos).magnitudeSquare();
		if (distance < ENTITY_FOLLOW_RANGE * ENTITY_FOLLOW_RANGE) {
			double speed = vel.magnitudeSquare();
			double stoppingDistance = (ENTITY_SLOWEST_SPEED * ENTITY_SLOWEST_SPEED - speed)/(2 * ENTITY_ACCELERATION);
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
			//Move in correct direction
			if (det < 0) {
				move.multiplyBy(ENTITY_ACCELERATION);
				flip = false;
			} else if (det > 0) {
				move.multiplyBy(-ENTITY_ACCELERATION);
				flip = true;
			} else {
				move.multiplyBy(0);
			}
			if (distance > stoppingDistance * stoppingDistance || speed<ENTITY_SLOWEST_SPEED * ENTITY_SLOWEST_SPEED) {
				vel.addTo(move);
				moving = true;
			} else {
				moving = false;
			}
		} else {
			moving = false;
		}
	}
	//Call inherited update (handles moving)
	Entity::update();
}
