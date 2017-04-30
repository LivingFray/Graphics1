#include "LogicGate.h"



LogicGate::LogicGate() {
	id = "logicgate";
	inChannelA = -1;
	inChannelB = -1;
	outChannel = -1;
	width = 0.25;
	height = 0.25;
	visible = false;
	editorMode = false;
	img = ImageLoader::getImage("Resources\\scenery\\logicGate.png");
}


LogicGate::~LogicGate() {
}


// Draws the logic gate
void LogicGate::draw(double ex) {
	if (visible || editorMode) {
		glPushMatrix();
		glColor4ub(255, 255, 255, visible ? 255 : 127);
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
}

// Sets the options for this selectable
void LogicGate::setOptions(OptionMenu* menu) {
	Scenery::setOptions(menu);
	std::map<string, string> values = menu->getValues();
	string v;
	v = values["Input Channel A"];
	inChannelA = atoi(v.c_str());
	v = values["Input Channel B"];
	inChannelB = atoi(v.c_str());
	v = values["Output Channel"];
	outChannel = atoi(v.c_str());
}


// Creates an option menu using the current values as defaults
void LogicGate::createOptions() {
	Scenery::createOptions();
	options->addOption("Input Channel A", true, to_string(inChannelA));
	options->addOption("Input Channel B", true, to_string(inChannelB));
	options->addOption("Output Channel", true, to_string(outChannel));
}


// Returns a DataObject representing the storable object
DataObject LogicGate::save() {
	DataObject sc = Scenery::save();
	sc.add("inChannelA", inChannelA);
	sc.add("inChannelB", inChannelB);
	sc.add("outChannel", outChannel);
	return sc;
}


// Loads the storable object from the DataObject
void LogicGate::load(DataObject obj) {
	Scenery::load(obj);
	inChannelA = obj.getInt("inChannelA");
	inChannelB = obj.getInt("inChannelB");
	outChannel = obj.getInt("outChannel");
}


// Returns if the selectable can be rotated
bool LogicGate::canResize() {
	return false;
}
