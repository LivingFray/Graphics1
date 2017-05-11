#include "TimedButton.h"
#include "Level.h"


TimedButton::TimedButton() {
	id = "timedbutton";
	activeTime = 1;
	cooldown = 0;
	nOffAnim.setSpriteSheet("Resources\\scenery\\button.png");
	nOnAnim.setSpriteSheet("Resources\\scenery\\button.png");
	sOffAnim.setSpriteSheet("Resources\\scenery\\buttonSelected.png");
	sOnAnim.setSpriteSheet("Resources\\scenery\\buttonSelected.png");
	offSound = SoundLoader::getSound("Resources\\sounds\\beep2.wav");
}


TimedButton::~TimedButton() {
	alDeleteSources(1, &offSound);
}


// Updates the object
void TimedButton::update() {
	selected = getNear();
	//Flip lever if key pressed
	if (KeyConfig::isDown("interact")) {
		if (!wasDown && selected && cooldown<=0) {
			cooldown = activeTime;
			alSourcePlay(clickSound);
		}
		wasDown = true;
	} else {
		wasDown = false;
	}
	//Update animation
	if (selected) {
		if (cooldown>0) {
			currentAnim = &sOnAnim;
		} else {
			currentAnim = &sOffAnim;
		}
	} else {
		if (cooldown>0) {
			currentAnim = &nOnAnim;
		} else {
			currentAnim = &nOffAnim;
		}
	}
	//Update channels
	if (cooldown>0) {
		Level* l = (Level*)state;
		l->setChannel(channel);
		cooldown -= TICKRATE;
		if (cooldown <= 0) {
			alSourcePlay(offSound);
		}
	}
}


// Sets the options for this selectable
void TimedButton::setOptions(OptionMenu* menu) {
	Lever::setOptions(menu);
	std::map<string, string> values = menu->getValues();
	string v;
	v = values["Duration"];
	activeTime = atof(v.c_str());
}


// Creates an option menu using the current values as defaults
void TimedButton::createOptions() {
	Lever::createOptions();
	options->addOption("Duration", true, to_string(activeTime));
}


// Returns a DataObject representing the storable object
DataObject TimedButton::save() {
	DataObject sc = Lever::save();
	sc.add("activeTime", activeTime);
	return sc;
}


// Loads the storable object from the DataObject
void TimedButton::load(DataObject obj) {
	Lever::load(obj);
	activeTime = obj.getDouble("activeTime");
}
