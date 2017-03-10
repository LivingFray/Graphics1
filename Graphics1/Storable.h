#pragma once
#include "DataObject.h"
#include "Globals.h"
class Storable {
public:
	Storable();
	~Storable();
	// Returns a DataObject representing the storable object
	virtual DataObject save();
	// Loads the storable object from the DataObject
	virtual void load(DataObject obj);
	// Gets the id of the object
	virtual string getId();
protected:
	string id;
};

