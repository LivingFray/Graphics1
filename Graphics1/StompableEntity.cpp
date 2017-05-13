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
	dead = false;
	deathSound = SoundLoader::getSound("Resources\\sounds\\error.wav");
}


StompableEntity::StompableEntity(const StompableEntity& other) : WorldObject(other), EntityAI(other) {
	idleAnim = other.idleAnim;
	currentAnim = &idleAnim;
	deadAnim = other.deadAnim;
	dead = other.dead;
	deathSound = SoundLoader::getSound("Resources\\sounds\\error.wav");
}


StompableEntity::~StompableEntity() {
	alDeleteSources(1, &deathSound);
}


// Called when damage is inflicted on the object
void StompableEntity::onDamage(Damage d) {
	if (!dead) {
		Level* l = (Level*)state;
		l->addScore(SCORE_KILL);
		dead = true;
		currentAnim = &deadAnim;
		alSourcePlay(deathSound);
	}
}


// Called when a collision occurs
void StompableEntity::onCollide(Collider* other) {
	//No corpses murdering players
	if (dead) {
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
			l->addScore(SCORE_KILL);
			dead = true;
			currentAnim = &deadAnim;
			alSourcePlay(deathSound);
		} else {
			other->onDamage(Damage::ENEMYCOLLISION);
		}
	}
}


// Updates the entity
void StompableEntity::update() {
	if (dead) {
		//Just handle moving/falling, no AI
		moving = false;
		Entity::update();
	} else {
		EntityAI::update();
	}
}
