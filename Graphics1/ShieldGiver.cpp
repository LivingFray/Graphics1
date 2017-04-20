#include "ShieldGiver.h"
#include "Level.h"

#define FOLLOW_SPEED 1
#define MAX_FOLLOW_SPEED 2

ShieldGiver::ShieldGiver() {
	id = "shieldgiver";
	following = NULL;
	currentAnim = Animation();
	currentAnim.setSpriteSheet("Resources\\entities\\shieldGiver.png");
	currentAnim.setSpritesheetSize(2, 2);
	currentAnim.addFrame(0, 0.05);
	currentAnim.addFrame(1, 0.05);
	currentAnim.addFrame(2, 0.05);
	currentAnim.addFrame(3, 0.05);
	currentAnim.addFrame(2, 0.05);
	currentAnim.addFrame(1, 0.05);
	currentAnim.setRepeat(true);
	currentAnim.setWidth(0.5);
	currentAnim.setHeight(0.5);
	width = 0.5;
	height = 0.5;
	deathTime = 0;
	lastTime = 0;
	dead = false;
	sparks = ParticleSystem();
	sparks.setTexture("Resources\\entities\\explosion.png");
	sparks.setMinSize(0.05);
	sparks.setMaxSize(0.05);
	sparks.setWidth(0);
	sparks.setHeight(0);
	sparks.setMinLife(0.5);
	sparks.setMaxLife(1);
	sparks.setParticlesPerSecond(2000);
	sparks.addColor(255, 255, 255, 255, 255, 255, 255, 0);
	sparks.addColor(125, 249, 255, 255, 125, 249, 255, 0);
}


ShieldGiver::~ShieldGiver() {
}



// Updates the player
void ShieldGiver::update() {
	if (dead) {
		deathTime += TICKRATE;
		if (deathTime > 1) {
			Level* l = (Level*)state;
			l->safeDelete(this);
		}
	} else if (following) {
		Vec2D dir = following->getPos().subtract(pos);
		Level* l = (Level*)state;
		Vec2D g;
		l->getGravityAtPos(following->getPos(), &g);
		dir.addTo(g.unit().multiply(-1.5));
		double d = dir.magnitudeSquare();
		vel.addTo(dir.unit().multiply(FOLLOW_SPEED));
		if (vel.magnitudeSquare() > MAX_FOLLOW_SPEED * MAX_FOLLOW_SPEED) {
			vel.toUnit();
			vel.multiplyBy(MAX_FOLLOW_SPEED);
		}
		pos.addTo(vel.multiply(TICKRATE));
	}
}


// Draws the player, ex seconds from last update
void ShieldGiver::draw(double ex) {
	if (dead) {
		sparks.setPosition(pos);
		deathTime += ex;
		if (deathTime > 0.05) {
			sparks.setEmitting(false);
		}
		sparks.draw(deathTime - lastTime);
		lastTime = deathTime;
		deathTime -= ex;
	} else {
		Entity::draw(ex);
	}
}


// Called when a collision occurs
void ShieldGiver::onCollide(Collider* other) {
	if (!following && other->getId() == "player") {
		Player* p = (Player*)other;
		following = p;
		p->giveShield(this);
	}
}


// Called when damage is inflicted on the object
void ShieldGiver::onDamage(Damage d) {
	//Instakill is a special damage type that can not be triggered through collision
	if (d == Damage::INSTAKILL) {
		dead = true;
		Level* l = (Level*)state;
		Vec2D g;
		l->getGravityAtPos(pos, &g);
		//Make sparks "floaty"
		g.multiplyBy(0.1);
		sparks.setGravity(g);
	}
}

// Returns whether the object is solid
bool ShieldGiver::isSolid() {
	return false;
}