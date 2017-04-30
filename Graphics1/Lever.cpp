#include "Lever.h"
#include "Level.h"


Lever::Lever() {
	id = "lever";
	width = 0.25;
	height = 0.25;
	nOffAnim = Animation();
	nOffAnim.setSpriteSheet("Resources\\entities\\lever.png");
	nOffAnim.setSize(0.25, 0.25);
	nOffAnim.setSpritesheetSize(2, 2);
	nOffAnim.setRepeat(false);
	nOffAnim.addFrame(0, 1);
	nOnAnim = Animation();
	nOnAnim.setSpriteSheet("Resources\\entities\\lever.png");
	nOnAnim.setSize(0.25, 0.25);
	nOnAnim.setSpritesheetSize(2, 2);
	nOnAnim.setRepeat(false);
	nOnAnim.addFrame(1, 1);
	sOffAnim = Animation();
	sOffAnim.setSpriteSheet("Resources\\entities\\lever.png");
	sOffAnim.setSize(0.25, 0.25);
	sOffAnim.setSpritesheetSize(2, 2);
	sOffAnim.setRepeat(false);
	sOffAnim.addFrame(2, 1);
	sOnAnim = Animation();
	sOnAnim.setSpriteSheet("Resources\\entities\\lever.png");
	sOnAnim.setSize(0.25, 0.25);
	sOnAnim.setSpritesheetSize(2, 2);
	sOnAnim.setRepeat(false);
	sOnAnim.addFrame(3, 1);
	currentAnim = &nOffAnim;
	//Disable gravity
	grav = 0;
	channel = 0;
	clickSound = SoundLoader::getSound("Resources\\sounds\\beep.wav");
}


Lever::~Lever() {
}


// Updates the entity
void Lever::update() {
	selected = getNear();
	//Flip lever if key pressed
	if (KeyConfig::isDown("interact")) {
		if (!wasDown) {
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
	Entity::setOptions(menu);
	std::map<string, string> values = menu->getValues();
	string v;
	v = values["Channel"];
	channel = atoi(v.c_str());
}


// Creates an option menu using the current values as defaults
void Lever::createOptions() {
	Entity::createOptions();
	options->addOption("Channel", true, to_string(channel));
}


// Returns a DataObject representing the storable object
DataObject Lever::save() {
	DataObject platform = Entity::save();
	platform.add("channel", channel);
	return platform;
}


// Loads the storable object from the DataObject
void Lever::load(DataObject obj) {
	Entity::load(obj);
	channel = obj.getInt("channel");
}
