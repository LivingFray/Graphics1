#include "ORGate.h"
#include "Level.h"


ORGate::ORGate() {
	id = "ORGate";
	img = ImageLoader::getImage("Resources\\scenery\\ORGate.png");
}


ORGate::~ORGate() {
}


// Updates the logic gate
void ORGate::update() {
	Level* l = (Level*)state;
	if (l->getChannel(inChannelA) || l->getChannel(inChannelB)) {
		l->setChannel(outChannel);
	}
}
