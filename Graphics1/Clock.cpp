#include "Clock.h"
#include "Level.h"


Clock::Clock() {
	id = "clock";
	width = 0.25;
	height = 0.25;
	img = ImageLoader::getImage("Resources\\scenery\\clock.png");
	channel = -1;
	on = false;
	time = 0;
}


Clock::~Clock() {
}


// Updates the object
void Clock::update() {
	time += TICKRATE;
	double t = (on ? onTime : offTime);
	if (time > t) {
		time -= t;
		on = !on;
	}
	if (on && !editorMode) {
		Level* l = (Level*)state;
		l->setChannel(channel);
	}
}

// Draws the timer
void Clock::draw(double ex) {
	if (editorMode) {
		glPushMatrix();
		glColor4ub(255, 255, 255, 127);
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
void Clock::setOptions(OptionMenu* menu) {
	Scenery::setOptions(menu);
	std::map<string, string> values = menu->getValues();
	string v;
	v = values["Channel"];
	channel = atoi(v.c_str());
	v = values["Inactive Duration"];
	offTime = atof(v.c_str());
	v = values["Active Duration"];
	onTime = atof(v.c_str());
}


// Creates an option menu using the current values as defaults
void Clock::createOptions() {
	Scenery::createOptions();
	options->addOption("Channel", true, to_string(channel));
	options->addOption("Inactive Duration", true, to_string(offTime));
	options->addOption("Active Duration", true, to_string(onTime));
}


// Returns a DataObject representing the storable object
DataObject Clock::save() {
	DataObject sc = Scenery::save();
	sc.add("channel", channel);
	sc.add("offTime", offTime);
	sc.add("onTime", onTime);
	return sc;
}


// Loads the storable object from the DataObject
void Clock::load(DataObject obj) {
	Scenery::load(obj);
	channel = obj.getInt("channel");
	offTime = obj.getDouble("offTime");
	onTime = obj.getDouble("onTime");
}


// Returns if the selectable can be resized
bool Clock::canResize() {
	return false;
}
