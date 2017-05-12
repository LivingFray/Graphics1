#include "PointGiver.h"
#include "Level.h"


PointGiver::PointGiver() {
	id = "points";
	width = 0.5;
	height = 0.5;
	anim = Animation();
	anim.setSpriteSheet("Resources\\entities\\pointGiver.png");
	anim.setSpritesheetSize(1, 1);
	anim.setSize(0.5, 0.5);
	anim.setRepeat(false);
	anim.addFrame(0, 1);
	currentAnim = &anim;
}


PointGiver::PointGiver(const PointGiver& other) : WorldObject(other), Entity(other) {
	anim = other.anim;
	currentAnim = &anim;
}

PointGiver::~PointGiver() {
}


// Called when a collision occurs
void PointGiver::onCollide(Collider* other) {
	if (other->getId() == "player") {
		Level* l = (Level*)state;
		l->addScore(SCORE_POINTGIVER);
		l->safeDelete(this);
	}
}


// Updates the entity
void PointGiver::update() {
}
