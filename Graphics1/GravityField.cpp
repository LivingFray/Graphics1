#include "GravityField.h"
#include "Globals.h"


GravityField::GravityField() {
	id = "gravfield";
	pos = Vec2D(0, 0);
	width = SMALLEST_THICKNESS;
	height = SMALLEST_THICKNESS;
	angle = 0;
	strength = 0;
}


GravityField::~GravityField() {
}

// Draws the gravity field ex seconds after last update
void GravityField::draw(double ex) {
	//TODO: Special effects
	double w = width / 2;
	double h = height / 2;
	glPushMatrix();
	glTranslated(pos.getX(), pos.getY(), 0.0);
	//0 degrees should be down, rather than right
	glRotated(angle, 0, 0, 1);
	glBegin(GL_QUADS);
	glColor3ub(0, 0, 255);
	glVertex2d(-1 * w, -1 * h);
	glVertex2d(w, -1 * h);
	glColor4ub(0, 0, 0, 0);
	glVertex2d(w, h);
	glVertex2d(-1 * w, h);
	glEnd();
	glPopMatrix();
}

// Called when the selectable is moved
bool GravityField::onMove(double dX, double dY) {
	pos.addTo(Vec2D(dX, dY));
	return true;
}


// Called when the selectable is resized
bool GravityField::onResize(double dX, double dY) {
	if (width + dX <= SMALLEST_THICKNESS) {
		return false;
	}
	if (height + dY <= SMALLEST_THICKNESS) {
		return false;
	}
	width += dX;
	height += dY;
	return true;
}


// Called when the selectable is rotated
bool GravityField::onRotate(double dAngle) {
	angle += dAngle;
	if (angle > 360) {
		angle -= 360;
	}
	if (angle < 0) {
		angle += 360;
	}
	return true;
}


// Returns if the selectable can be moved
bool GravityField::canMove() {
	return true;
}


// Returns if the selectable can be rotated
bool GravityField::canResize() {
	return true;
}


// Returns if the selectable can be rotated
bool GravityField::canRotate() {
	return true;
}


// Returns if the selectable is selected
bool GravityField::isInBoundingBox(double x, double y) {
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


// Returns the centre of the selectable
Vec2D GravityField::getPos() {
	return pos;
}


// Sets the position of the selectable
void GravityField::setPos(Vec2D pos) {
	this->pos = pos;
}


// Gets the angle of the selectable
double GravityField::getAngle() {
	return angle;
}


// Gets the width of the selectable
double GravityField::getWidth() {
	return width;
}


// Gets the height of the selectable
double GravityField::getHeight() {
	return height;
}


// Gets the strength of the gravity field
double GravityField::getStrength() {
	return strength;
}


// Gets the angle of the selectable
void GravityField::setAngle(double angle) {
	this->angle = angle;
}


// Gets the width of the selectable
void GravityField::setWidth(double width) {
	this->width = width;
}


// Gets the height of the selectable
void GravityField::setHeight(double height) {
	this->height = height;
}


// Sets the strength of the gravity field
void GravityField::setStrength(double strength) {
	this->strength = strength;
}


// Returns a DataObject representing the storable object
DataObject* GravityField::save() {
	DataObject* field = new DataObject();
	//Magically convert to pointer to avoid dereferencing
	double* x = new double(pos.getX());
	double* y = new double(pos.getY());
	field->add("id", STRING, &id);
	field->add("x", DOUBLE, x);
	field->add("y", DOUBLE, y);
	field->add("width", DOUBLE, &width);
	field->add("height", DOUBLE, &height);
	field->add("angle", DOUBLE, &angle);
	field->add("strength", DOUBLE, &strength);
	return field;
}


// Loads the storable object from the DataObject
void GravityField::load(DataObject* obj) {
	DATATYPE type;
	void* data;
	//Default values
	double x = 0.0, y = 0.0;
	double w = 0.0, h = 0.0;
	double a = 0.0, s = 0.0;
	//Load in values where possible
	data = obj->get("x", type);
	if (type == DOUBLE) {
		x = *(double*)data;
	}
	data = obj->get("y", type);
	if (type == DOUBLE) {
		y = *(double*)data;
	}
	data = obj->get("width", type);
	if (type == DOUBLE) {
		w = *(double*)data;
	}
	data = obj->get("height", type);
	if (type == DOUBLE) {
		h = *(double*)data;
	}
	data = obj->get("angle", type);
	if (type == DOUBLE) {
		a = *(double*)data;
	}
	data = obj->get("strength", type);
	if (type == DOUBLE) {
		s = *(double*)data;
	}
	pos = Vec2D(x, y);
	width = w>0 ? w : 0;
	height = h>0 ? h : 0;
	angle = a;
	strength = s;
}
