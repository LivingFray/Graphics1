#include "Light.h"
#include "Level.h"


Light::Light() {
	id = "light";
	width = 0.25;
	height = 0.25;
	imgOff = ImageLoader::getImage("Resources\\scenery\\lightOff.png");
	imgOn = ImageLoader::getImage("Resources\\scenery\\lightOn.png");
	channel = 0;
}


Light::~Light() {
}


// Draws the logic gate
void Light::draw(double ex) {
	Level* l = (Level*)state;
	GLuint img;
	if (editorMode) {
		img = imgOff;
	} else {
		img = l->getChannel(channel) ? imgOn : imgOff;
	}
	glPushMatrix();
	glColor4ub(255, 255, 255, 255);
	glTranslated(pos.getX(), pos.getY(), 0.0);
	glRotated(angle, 0.0, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, img);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex2d(-width * 0.5, -height * 0.5);
	glTexCoord2d(1, 0);
	glVertex2d(width * 0.5, -height * 0.5);
	glTexCoord2d(1, 1);
	glVertex2d(width * 0.5, height * 0.5);
	glTexCoord2d(0, 1);
	glVertex2d(-width * 0.5, height * 0.5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

// Sets the options for this selectable
void Light::setOptions(OptionMenu* menu) {
	Scenery::setOptions(menu);
	std::map<string, string> values = menu->getValues();
	string v;
	v = values["Channel"];
	channel = atoi(v.c_str());
}


// Creates an option menu using the current values as defaults
void Light::createOptions() {
	Scenery::createOptions();
	options->addOption("Channel", true, to_string(channel));
}


// Returns a DataObject representing the storable object
DataObject Light::save() {
	DataObject sc = Scenery::save();
	sc.add("channel", channel);
	return sc;
}


// Loads the storable object from the DataObject
void Light::load(DataObject obj) {
	Scenery::load(obj);
	channel = obj.getInt("channel");
}


// Whether the object can be resized
bool Light::canResize() {
	return false;
}