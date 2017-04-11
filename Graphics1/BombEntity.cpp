#include "BombEntity.h"
#include "Level.h"
#include "Explosion.h"


#define TRIGGER_RADIUS 2.0
#define FUSE_TIME 4.0
#define BOMB_FLASHES 5

BombEntity::BombEntity() {
	id = "bomb";
	currentAnim = Animation();
	currentAnim.setSpriteSheet("Resources\\entities\\bomb.png");
	currentAnim.setSpritesheetSize(2, 1);
	currentAnim.setSize(0.5, 0.5);
	currentAnim.setRepeat(true);
	currentAnim.addFrame(0, 0.5);
	currentAnim.addFrame(1, 0.5);
	width = 0.5;
	height = 0.5;
	explodeAnim = Animation();
	explodeAnim.setSpriteSheet("Resources\\entities\\bomb.png");
	explodeAnim.setSpritesheetSize(2, 1);
	explodeAnim.setSize(0.5, 0.5);
	explodeAnim.setRepeat(false);
	//Each frame lasts half as long as the last
	double time = 0.5 * (FUSE_TIME / 2.0);
	for (int i = 0; i < BOMB_FLASHES; i++) {
		explodeAnim.addFrame(0, time);
		explodeAnim.addFrame(1, time);
		time *= 0.5;
	}
	triggered = false;
	fuse = FUSE_TIME;
}


BombEntity::~BombEntity() {
}


//Updates the entity
void BombEntity::update() {
	Level* l = (Level*)state;
	Entity* player = l->getPlayer();
	//Explode if in range
	if (!triggered && player != NULL) {
		//Check distance to player
		double d = player->getPos().subtract(pos).magnitudeSquare();
		if (d < TRIGGER_RADIUS * TRIGGER_RADIUS) {
			triggered = true;
			currentAnim = explodeAnim;
			explodeAnim.setTime(0);
		}
	} else if (triggered) {
		fuse -= TICKRATE;
		if (fuse <= 0) {
			explode();
		}
	}
	EntityAI::update();
}


void BombEntity::explode() {
	Level* l = (Level*)state;
	l->safeDelete(this);
	Explosion* exp = new Explosion();
	exp->setRadius(2);
	exp->setPos(pos);
	exp->setMaxAge(1);
	l->safeAdd(exp);
}


// Called when damage is inflicted on the object
void BombEntity::onDamage(Damage d) {
	if (d == Damage::EXPLOSION) {
		explode();
	}
}