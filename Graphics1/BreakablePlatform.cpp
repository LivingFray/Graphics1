#include "BreakablePlatform.h"
#include "Level.h"


BreakablePlatform::BreakablePlatform() {
	id = "breakable";
}


BreakablePlatform::~BreakablePlatform() {
}


// Called when damage is inflicted on the object
void BreakablePlatform::onDamage(Damage d) {
	Level* l = (Level*)state;
	l->safeDelete(this);
}
