#include "XNORGate.h"
#include "Level.h"


XNORGate::XNORGate() {
	id = "XNORGate";
	img = ImageLoader::getImage("Resources\\scenery\\XNORGate.png");
}


XNORGate::~XNORGate() {
}



// Updates the logic gate
void XNORGate::update() {
	Level* l = (Level*)state;
	if ((l->getChannel(inChannelA) && l->getChannel(inChannelB)) || !(l->getChannel(inChannelA) || l->getChannel(inChannelB))) {
		l->setChannel(outChannel);
	}
}
