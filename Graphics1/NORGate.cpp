#include "NORGate.h"
#include "Level.h"


NORGate::NORGate() {
	id = "NORGate";
	img = ImageLoader::getImage("Resources\\scenery\\NORGate.png");
}


NORGate::~NORGate() {
}


// Updates the logic gate
void NORGate::update() {
	Level* l = (Level*)state;
	if (!(l->getChannel(inChannelA) || l->getChannel(inChannelB))) {
		l->setChannel(outChannel);
	}
}
