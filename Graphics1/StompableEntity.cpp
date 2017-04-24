#include "StompableEntity.h"
#include "Level.h"


StompableEntity::StompableEntity() {
	id = "stomp";
	width = 1.0;
	height = 1.0;
	idleAnim = Animation();
	idleAnim.setSpriteSheet("Resources\\entities\\stompAlive.png");
	idleAnim.setSpritesheetSize(1, 1);
	idleAnim.setSize(1.0,1.0);
	idleAnim.addFrame(0, 1);
	idleAnim.setRepeat(true);
	currentAnim = &idleAnim;
	deadAnim = Animation();
	deadAnim.setSpriteSheet("Resources\\entities\\stompDead.png");
	deadAnim.setSpritesheetSize(1, 1);
	deadAnim.setSize(1.0, 1.0);
	deadAnim.addFrame(0, 1);
	deadAnim.setRepeat(false);
	deadTime = 0;
}


StompableEntity::~StompableEntity() {
}


// Called when damage is inflicted on the object
void StompableEntity::onDamage(Damage d) {
	deadTime = ENTITY_DEAD_TIME;
	currentAnim = &deadAnim;
}


// Called when a collision occurs
void StompableEntity::onCollide(Collider* other) {
	//No corpses murdering players
	if (deadTime != 0) {
		return;
	}
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
			deadTime = ENTITY_DEAD_TIME;
			currentAnim = &deadAnim;
		} else {
			other->onDamage(Damage::ENEMYCOLLISION);
		}
	}
}


// Updates the entity
void StompableEntity::update() {
	if (deadTime > 0) {
		deadTime -= TICKRATE;
		if (deadTime <= 0) {
			Level* l = (Level*)state;
			l->safeDelete(this);
		}
		//Just handle moving/falling, no AI
		moving = false;
		Entity::update();
	} else {
		EntityAI::update();
	}
}
