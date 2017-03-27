#include "Storable.h"



Storable::Storable() {
}


Storable::~Storable() {
}


// Returns a DataObject representing the storable object
DataObject Storable::save() {
	DataObject obj = DataObject();
	obj.add("id", id);
	if (canMove()) {
		obj.add("x", pos.getX());
		obj.add("y", pos.getY());
		obj.add("velX", vel.getX());
		obj.add("velY", vel.getY());
	}
	if (canResize()) {
		obj.add("width", width);
		obj.add("height", height);
	}
	if (canRotate()) {
		obj.add("angle", angle);
	}
	return obj;
}


// Loads the storable object from the DataObject
void Storable::load(DataObject obj) {
	if (canMove()) {
		pos.setX(obj.getDouble("x"));
		pos.setY(obj.getDouble("y"));
		vel.setX(obj.getDouble("velX"));
		vel.setY(obj.getDouble("velY"));
	}
	if (canResize()) {
		setWidth(obj.getDouble("width"));
		setHeight(obj.getDouble("height"));
	}
	if (canRotate()) {
		setAngle(obj.getDouble("angle"));
	}
}
