#include "PointGiver.h"
#include "Level.h"


PointGiver::PointGiver() {
	id = "points";
	width = 0.2;
	height = 0.2;
	anim = Animation();
	anim.setSpriteSheet("Resources\\entities\\pointGiver.png");
	anim.setSpritesheetSize(2, 2);
	anim.setSize(0.2, 0.2);
	//Temp
	anim.addFrame(0, 1);
	anim.addFrame(1, 1);
	currentAnim = &anim;
}


PointGiver::~PointGiver() {
}


// Called when a collision occurs
void PointGiver::onCollide(Collider* other) {
	if (other->getId() == "player") {
		Level* l = (Level*)state;
		l->addScore(ITEM_SCORE_VALUE);
		l->safeDelete(this);
	}
}


// Updates the entity
void PointGiver::update() {
}