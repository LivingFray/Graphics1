#include "Selectable.h"



Selectable::Selectable() {
	options = OptionMenu();
}


Selectable::~Selectable() {
}


// Called when the selectable is moved
bool Selectable::onMove(double dX, double dY) {
	pos.addTo(Vec2D(dX, dY));
	return true;
}


// Called when the selectable is resized
bool Selectable::onResize(double dX, double dY) {
	if (width + dX <= SMALLEST_THICKNESS) {
		return false;
	}
	if (height + dY <= SMALLEST_THICKNESS) {
		return false;
	}
	setWidth(width + dX);
	setHeight(height + dY);
	return true;
}


// Called when the selectable is rotated
bool Selectable::onRotate(double dAngle) {
	angle += dAngle;
	if (angle > 360) {
		angle -= 360;
	}
	if (angle < 0) {
		angle += 360;
	}
	return true;
}


// Returns if the selectable can be deleted
bool Selectable::canDelete() {
	return true;
}


// Returns if the selectable is selected
bool Selectable::isInBoundingBox(double x, double y) {
	//Translate point to be relative to the BB's centre
	Vec2D p = Vec2D(x, y).subtract(pos);
	//Rotate the point back to be AA with the BB (-angle)
	double cTheta = cos(-DEG_TO_RAD * angle);
	double sTheta = sin(-DEG_TO_RAD * angle);
	double xPrime = p.getX() * cTheta - p.getY() * sTheta;
	double yPrime = p.getY() * cTheta + p.getX() * sTheta;
	//Calculate field strength at point
	return xPrime >= -width / 2 && xPrime <= width / 2
		&& yPrime >= -height / 2 && yPrime <= height / 2;
}


// Gets the options screen for this selectable
OptionMenu* Selectable::getOptions() {
	return &options;
}


// Sets the options for this selectable
void Selectable::setOptions(OptionMenu* menu) {
	string v;
	std::map<string, string> values = menu->getValues();
	if (canMove()) {
		v = values["X Position"];
		pos.setX(atof(v.c_str()));
		v = values["Y Position"];
		pos.setY(atof(v.c_str()));
	}
	if (canResize()) {
		v = values["Width"];
		width = atof(v.c_str());
		v = values["Height"];
		height = atof(v.c_str());
	}
	if (canRotate()) {
		v = values["Angle"];
		angle = atof(v.c_str());
	}
}


// Creates an option menu using the current values as defaults
void Selectable::createOptions() {
	//Clear out options
	options = OptionMenu();
	//Add default options
	if (canMove()) {
		options.addOption("X Position", true, to_string(getPos().getX()));
		options.addOption("Y Position", true, to_string(getPos().getY()));
	}
	if (canResize()) {
		options.addOption("Width", true, to_string(getWidth()));
		options.addOption("Height", true, to_string(getHeight()));
	}
	if (canRotate()) {
		options.addOption("Angle", true, to_string(getAngle()));
	}
}
