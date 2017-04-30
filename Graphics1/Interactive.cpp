#include "Interactive.h"
#include "Globals.h"
#include "Level.h"

Interactive::Interactive() {
	id = "interactive";
}


Interactive::~Interactive() {
}


//Checks whether the player is within range
bool Interactive::getNear() {
	Level* l = (Level*)state;
	if (l->getPlayer()) {
		return l->getPlayer()->getPos().subtract(pos).magnitudeSquare() < INTERACTION_RANGE * INTERACTION_RANGE;
	}
	return false;
}
