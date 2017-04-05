#include "PointGiver.h"
#include "Level.h"


PointGiver::PointGiver() {
	id = "points";
	width = 0.2;
	height = 0.2;
	currentAnim = Animation();
	currentAnim.setSpriteSheet("Resources\\entities\\pointGiver.png");
	currentAnim.setSpritesheetSize(2, 2);
	currentAnim.setSize(0.2, 0.2);
	//Temp
	currentAnim.addFrame(0, 1);
	currentAnim.addFrame(1, 1);
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