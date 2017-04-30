#include "XORGate.h"
#include "Level.h"


XORGate::XORGate() {
	id = "XORGate";
	img = ImageLoader::getImage("Resources\\scenery\\XORGate.png");
}


XORGate::~XORGate() {
}


// Updates the logic gate
void XORGate::update() {
	Level* l = (Level*)state;
	if ((l->getChannel(inChannelA) || l->getChannel(inChannelB)) && !(l->getChannel(inChannelA) && l->getChannel(inChannelB))) {
		l->setChannel(outChannel);
	}
}
