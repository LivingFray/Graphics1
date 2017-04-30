#include "PhasingPlatform.h"
#include "Level.h"


PhasingPlatform::PhasingPlatform() {
	id = "phase";
	channel = 0;
}


PhasingPlatform::~PhasingPlatform() {
}


bool PhasingPlatform::isSolid() {
	Level* l = (Level*)state;
	return editorMode || !l->getChannel(channel);
}


// Returns a DataObject representing the storable object
DataObject PhasingPlatform::save() {
	DataObject platform = Platform::save();
	platform.add("channel", channel);
	return platform;
}


// Loads the storable object from the DataObject
void PhasingPlatform::load(DataObject obj) {
	Platform::load(obj);
	channel = obj.getInt("channel");
}


// Sets the options for this selectable
void PhasingPlatform::setOptions(OptionMenu* menu) {
	Platform::setOptions(menu);
	std::map<string, string> values = menu->getValues();
	string v;
	v = values["Channel"];
	channel = atoi(v.c_str());
}


// Creates an option menu using the current values as defaults
void PhasingPlatform::createOptions() {
	Platform::createOptions();
	options->addOption("Channel", true, to_string(channel));
}


// Draws the platform ex seconds after last update
void PhasingPlatform::draw(double ex) {
	glColor4ub(255, 255, 255, (isSolid() ? 255 : 127));
	glPushMatrix();
	glTranslated(pos.getX() + vel.getX()*ex, pos.getY() + vel.getY()*ex, 0.0);
	glRotated(angle, 0.0, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(-width / 2, -height / 2);
	glTexCoord2d(0.0, texY);
	glVertex2d(-width / 2, height / 2);
	glTexCoord2d(texX, texY);
	glVertex2d(width / 2, height / 2);
	glTexCoord2d(texX, 0.0);
	glVertex2d(width / 2, -height / 2);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
