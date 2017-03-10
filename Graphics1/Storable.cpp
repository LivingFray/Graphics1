#include "Storable.h"



Storable::Storable() {
}


Storable::~Storable() {
}


// Returns a DataObject representing the storable object
DataObject Storable::save() {
	return DataObject();
}


// Loads the storable object from the DataObject
void Storable::load(DataObject obj) {
}


// Gets the id of the object
string Storable::getId() {
	return id;
}
