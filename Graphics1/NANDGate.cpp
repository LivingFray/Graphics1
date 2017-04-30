#include "NANDGate.h"
#include "Level.h"


NANDGate::NANDGate() {
	id = "NANDGate";
	img = ImageLoader::getImage("Resources\\scenery\\NANDGate.png");
}


NANDGate::~NANDGate() {
}


// Updates the logic gate
void NANDGate::update() {
	Level* l = (Level*)state;
	if (!(l->getChannel(inChannelA) && l->getChannel(inChannelB))) {
		l->setChannel(outChannel);
	}
}
