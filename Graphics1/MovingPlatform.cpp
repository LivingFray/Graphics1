#include "MovingPlatform.h"
#include "Level.h"


MovingPlatform::MovingPlatform() {
	id = "movingplatform";
	currentNode = 1;
	travelTime = 1;
	progress = 0;
	channel = -1;
}


MovingPlatform::~MovingPlatform() {
}


// Sets the first node the platform travels to
void MovingPlatform::setFirstNode(Vec2D node) {
	nodes[0] = node;
}


// Sets the last node the platform travels to
void MovingPlatform::setLastNode(Vec2D node) {
	nodes[1] = node;
}


// Gets the first node the platfrom travels to
Vec2D MovingPlatform::getFirstNode() {
	return nodes[0];
}


// Gets the last node the platform travels to
Vec2D MovingPlatform::getLastNode() {
	return nodes[1];
}


// Sets the travel time from one node to the other
void MovingPlatform::setTravelTime(double t) {
	if (t > 0) {
		travelTime = t;
	}
}


// Gets the travel time from one node to the other
double MovingPlatform::getTravelTime() {
	return travelTime;
}


// Returns a DataObject representing the storable object
DataObject MovingPlatform::save() {
	DataObject platform = Platform::save();
	platform.add("firstNodeX", nodes[0].getX());
	platform.add("firstNodeY", nodes[0].getY());
	platform.add("lastNodeX", nodes[1].getX());
	platform.add("lastNodeY", nodes[1].getY());
	platform.add("travelTime", travelTime);
	platform.add("channel", channel);
	return platform;
}


// Loads the storable object from the DataObject
void MovingPlatform::load(DataObject obj) {
	Platform::load(obj);
	nodes[0].setX(obj.getDouble("firstNodeX"));
	nodes[0].setY(obj.getDouble("firstNodeY"));
	nodes[1].setX(obj.getDouble("lastNodeX"));
	nodes[1].setY(obj.getDouble("lastNodeY"));
	setTravelTime(obj.getDouble("travelTime"));
	channel = obj.getInt("channel");
}


// Sets the options for this selectable
void MovingPlatform::setOptions(OptionMenu* menu) {
	Platform::setOptions(menu);
	std::map<string, string> values = menu->getValues();
	string v;
	v = values["First Node X"];
	nodes[0].setX(atof(v.c_str()));
	v = values["First Node Y"];
	nodes[0].setY(atof(v.c_str()));
	v = values["Last Node X"];
	nodes[1].setX(atof(v.c_str()));
	v = values["Last Node Y"];
	nodes[1].setY(atof(v.c_str()));
	v = values["Travel Time"];
	setTravelTime(atof(v.c_str()));
	v = values["Channel"];
	channel = atoi(v.c_str());
}


// Creates an option menu using the current values as defaults
void MovingPlatform::createOptions() {
	Platform::createOptions();
	options->addOption("First Node X", true, to_string(nodes[0].getX()));
	options->addOption("First Node Y", true, to_string(nodes[0].getY()));
	options->addOption("Last Node X", true, to_string(nodes[1].getX()));
	options->addOption("Last Node Y", true, to_string(nodes[1].getY()));
	options->addOption("Travel Time", true, to_string(travelTime));
	options->addOption("Channel", true, to_string(channel));
}


// Updates the platform
void MovingPlatform::update() {
	Level* l = (Level*)state;
	if (l->getChannel(channel)) {
		vel.set(cVel.getX(), cVel.getY());
		progress += TICKRATE;
		if (progress > travelTime) {
			//Make sure platform doesn't overshoot
			vel = nodes[currentNode].subtract(pos).divide(TICKRATE);
			//Change target
			currentNode++;
			currentNode %= 2;
			//Reset progress
			progress = 0;
		} else if (progress == TICKRATE) {
			//Start moving
			vel = nodes[currentNode].subtract(pos).divide(travelTime);
			cVel.set(vel.getX(), vel.getY());
		}
	} else {
		vel = Vec2D(0.0, 0.0);
	}
	//Move platform and handle any other stuff
	Platform::update();
}


// Called when the selectable is moved
bool MovingPlatform::onMove(double dX, double dY) {
	bool r = Platform::onMove(dX, dY);
	nodes[0] = pos;
	return r;
}

// Draws the platform
void MovingPlatform::draw(double ex) {
	if (editorMode) {
		glColor4ub(255, 255, 255, 127);
		glPushMatrix();
		glTranslated(nodes[1].getX(), nodes[1].getY(), 0.0);
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
	Platform::draw(ex);
}