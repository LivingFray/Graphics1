#include "ANDGate.h"
#include "Level.h"


ANDGate::ANDGate() {
	id = "ANDGate";
	img = ImageLoader::getImage("Resources\\scenery\\ANDGate.png");
}


ANDGate::~ANDGate() {
}


// Updates the logic gate
void ANDGate::update() {
	Level* l = (Level*)state;
	if (l->getChannel(inChannelA) && l->getChannel(inChannelB)) {
		l->setChannel(outChannel);
	}
}
