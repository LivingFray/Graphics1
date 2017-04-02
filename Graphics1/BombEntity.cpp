#include "BombEntity.h"
#include "Level.h"


BombEntity::BombEntity() {
	id = "bomb";
	currentAnim = Animation();
	currentAnim.setSpriteSheet("Resources\\entities\\bomb.png");
	currentAnim.setSpritesheetSize(2, 1);
	currentAnim.setSize(0.5, 0.5);
	currentAnim.setRepeat(false);
	currentAnim.addFrame(0, 0.5);
	currentAnim.addFrame(1, 0.5);
	width = 0.5;
	height = 0.5;
}


BombEntity::~BombEntity() {
}


//Updates the entity
void BombEntity::update() {
	Level* l = (Level*)state;
	Entity* player = l->getPlayer();
	//TODO: Explosion if in range
	EntityAI::update();
}