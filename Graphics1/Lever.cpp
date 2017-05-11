#include "Lever.h"
#include "Level.h"


Lever::Lever() {
	id = "lever";
	width = 0.25;
	height = 0.25;
	nOffAnim = Animation();
	nOffAnim.setSpriteSheet("Resources\\scenery\\lever.png");
	nOffAnim.setSize(0.25, 0.25);
	nOffAnim.setSpritesheetSize(2, 1);
	nOffAnim.setRepeat(false);
	nOffAnim.addFrame(0, 1);
	nOnAnim = Animation();
	nOnAnim.setSpriteSheet("Resources\\scenery\\lever.png");
	nOnAnim.setSize(0.25, 0.25);
	nOnAnim.setSpritesheetSize(2, 1);
	nOnAnim.setRepeat(false);
	nOnAnim.addFrame(1, 1);
	sOffAnim = Animation();
	sOffAnim.setSpriteSheet("Resources\\scenery\\leverSelected.png");
	sOffAnim.setSize(0.25, 0.25);
	sOffAnim.setSpritesheetSize(2, 1);
	sOffAnim.setRepeat(false);
	sOffAnim.addFrame(0, 1);
	sOnAnim = Animation();
	sOnAnim.setSpriteSheet("Resources\\scenery\\leverSelected.png");
	sOnAnim.setSize(0.25, 0.25);
	sOnAnim.setSpritesheetSize(2, 1);
	sOnAnim.setRepeat(false);
	sOnAnim.addFrame(1, 1);
	currentAnim = &nOffAnim;
	channel = 0;
	clickSound = SoundLoader::getSound("Resources\\sounds\\beep.wav");
}


Lever::~Lever() {
	alDeleteSources(1, &clickSound);
}


// Updates the entity
void Lever::update() {
	selected = getNear();
	//Flip lever if key pressed
	if (KeyConfig::isDown("interact")) {
		if (!wasDown && selected) {
			active = !active;
			alSourcePlay(clickSound);
		}
		wasDown = true;
	} else {
		wasDown = false;
	}
	//Update animation
	if (selected) {
		if (active) {
			currentAnim = &sOnAnim;
		} else {
			currentAnim = &sOffAnim;
		}
	} else {
		if (active) {
			currentAnim = &nOnAnim;
		} else {
			currentAnim = &nOffAnim;
		}
	}
	//Update channels
	if (active) {
		Level* l = (Level*)state;
		l->setChannel(channel);
	}
}


// Sets the options for this selectable
void Lever::setOptions(OptionMenu* menu) {
	Scenery::setOptions(menu);
	std::map<string, string> values = menu->getValues();
	string v;
	v = values["Channel"];
	channel = atoi(v.c_str());
}


// Creates an option menu using the current values as defaults
void Lever::createOptions() {
	Scenery::createOptions();
	options->addOption("Channel", true, to_string(channel));
}


// Returns a DataObject representing the storable object
DataObject Lever::save() {
	DataObject sc = Scenery::save();
	sc.add("channel", channel);
	return sc;
}


// Loads the storable object from the DataObject
void Lever::load(DataObject obj) {
	Scenery::load(obj);
	channel = obj.getInt("channel");
}


// Returns if the selectable can be rotated
bool Lever::canResize() {
	return false;
}
