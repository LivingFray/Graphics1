#include "Crate.h"



Crate::Crate() {
	id = "crate";
	anim = Animation();
	width = 1;
	height = 1;
	anim.setSpriteSheet("Resources\\entities\\crate.png");
	anim.setSpritesheetSize(1, 1);
	anim.setSize(width, height);
	anim.addFrame(0, 1);
	anim.setRepeat(false);
	anim.setTime(0);
	currentAnim = &anim;
}


Crate::Crate(const Crate& other) : WorldObject(other), Entity(other) {
	anim = other.anim;
	currentAnim = &anim;
}


Crate::~Crate() {
}
