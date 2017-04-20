#include "StompableEntity.h"
#include "Level.h"


StompableEntity::StompableEntity() {
	id = "stomp";
	width = 0.5;
	height = 0.5;
	currentAnim = Animation();
	currentAnim.setSpriteSheet("Resources\\entities\\stomp.png");
	currentAnim.setSpritesheetSize(1, 1);
	currentAnim.setSize(0.5, 0.5);
	currentAnim.addFrame(0, 1);
	currentAnim.setRepeat(true);
}


StompableEntity::~StompableEntity() {
}


// Called when damage is inflicted on the object
void StompableEntity::onDamage(Damage d) {
	Level* l = (Level*)state;
	l->safeDelete(this);
}

// Called when a collision occurs
void StompableEntity::onCollide(Collider* other) {
	if (other->getId() == "player") {
		Level* l = (Level*)state;
		Vec2D grav;
		l->getGravityAtPos(pos, &grav);
		//Calculate if stomped on or not
		Vec2D dir = other->getPos().subtract(pos);
		//If player is above entity
		bool stomped = (dir.dot(grav) / (grav.magnitude() * dir.magnitude()))<0;
		//If player is moving down
		stomped = stomped && (other->getVel().dot(grav) / (grav.magnitude() * other->getVel().magnitude()))>0;
		if (stomped) {
			//TODO: Death anim
			l->safeDelete(this);
		} else {
			other->onDamage(Damage::ENEMYCOLLISION);
		}
	}
}