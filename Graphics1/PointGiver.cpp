#include "PointGiver.h"
#include "Level.h"


PointGiver::PointGiver() {
	id = "points";
	width = 20;
	height = 20;
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