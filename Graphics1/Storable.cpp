#include "Storable.h"



Storable::Storable() {
}


Storable::~Storable() {
}


// Returns a DataObject representing the storable object
DataObject Storable::save() {
	DataObject obj = DataObject();
	obj.add("id", id);
	obj.add("x", pos.getX());
	obj.add("y", pos.getY());
	obj.add("width", width);
	obj.add("height", height);
	obj.add("angle", angle);
	obj.add("velX", vel.getX());
	obj.add("velY", vel.getY());
	return obj;
}


// Loads the storable object from the DataObject
void Storable::load(DataObject obj) {
	pos.setX(obj.getDouble("x"));
	pos.setY(obj.getDouble("y"));
	setWidth(obj.getDouble("width"));
	setHeight(obj.getDouble("height"));
	setAngle(obj.getDouble("angle"));
	vel.setX(obj.getDouble("velX"));
	vel.setY(obj.getDouble("velY"));
}
