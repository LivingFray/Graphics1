#include "ButtonEntity.h"
#include "Level.h"


ButtonEntity::ButtonEntity() {
	id = "button";
	grav = 0;
	channel = -1;
	width = 1.0;
	height = 0.5;
	upAnim = Animation();
	upAnim.setSpriteSheet("Resources\\entities\\button.png");
	upAnim.setSpritesheetSize(1, 2);
	upAnim.addFrame(0, 1);
	upAnim.setWidth(width);
	upAnim.setHeight(height);
	upAnim.setRepeat(true);
	downAnim = Animation();
	downAnim.setSpriteSheet("Resources\\entities\\button.png");
	downAnim.setSpritesheetSize(1, 2);
	downAnim.addFrame(1, 1);
	downAnim.setWidth(width);
	downAnim.setHeight(height);
	downAnim.setRepeat(true);
	currentAnim = &upAnim;
}


ButtonEntity::ButtonEntity(const ButtonEntity& other) : WorldObject(other), Entity(other) {
	channel = other.channel;
	grav = other.grav;
	upAnim = other.upAnim;
	downAnim = other.downAnim;
	currentAnim = &upAnim;
}


ButtonEntity::~ButtonEntity() {

}


// Updates the entity
void ButtonEntity::update() {
	currentAnim = &upAnim;
}


// Called when a collision occurs
void ButtonEntity::onCollide(Collider* other) {
	Level* l = (Level*)state;
	l->setChannel(channel);
	currentAnim = &downAnim;
}


// Sets the options for this selectable
void ButtonEntity::setOptions(OptionMenu* menu) {
	Entity::setOptions(menu);
	std::map<string, string> values = menu->getValues();
	string v;
	v = values["Channel"];
	channel = atoi(v.c_str());
}


// Creates an option menu using the current values as defaults
void ButtonEntity::createOptions() {
	Entity::createOptions();
	options->addOption("Channel", true, to_string(channel));
}


// Returns a DataObject representing the storable object
DataObject ButtonEntity::save() {
	DataObject sc = Entity::save();
	sc.add("channel", channel);
	return sc;
}


// Loads the storable object from the DataObject
void ButtonEntity::load(DataObject obj) {
	Entity::load(obj);
	channel = obj.getInt("channel");
}
