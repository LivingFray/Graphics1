#include "Selectable.h"



Selectable::Selectable() {
	options = OptionMenu();
}


Selectable::~Selectable() {
}


// Called when the selectable is moved
bool Selectable::onMove(double dX, double dY) {
	return false;
}


// Called when the selectable is resized
bool Selectable::onResize(double dX, double dY) {
	return false;
}


// Called when the selectable is rotated
bool Selectable::onRotate(double dAngle) {
	return false;
}


// Returns if the selectable can be moved
bool Selectable::canMove() {
	return true;
}


// Returns if the selectable can be rotated
bool Selectable::canResize() {
	return true;
}


// Returns if the selectable can be rotated
bool Selectable::canRotate() {
	return true;
}


// Returns if the selectable can be deleted
bool Selectable::canDelete() {
	return true;
}


// Returns if the selectable is selected
bool Selectable::isInBoundingBox(double x, double y) {
	return false;
}


// Returns the centre of the selectable
Vec2D Selectable::getPos() {
	return Vec2D(0, 0);
}


// Sets the position of the selectable
void Selectable::setPos(Vec2D pos) {
}


// Gets the angle of the selectable
double Selectable::getAngle() {
	return 0.0;
}


// Gets the width of the selectable
double Selectable::getWidth() {
	return 0.0;
}


// Gets the height of the selectable
double Selectable::getHeight() {
	return 0.0;
}


// Gets the options screen for this selectable
OptionMenu* Selectable::getOptions() {
	return &options;
}


// Sets the options for this selectable
void Selectable::setOptions(OptionMenu* menu) {

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
