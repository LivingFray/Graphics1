#pragma once
#include "DataObject.h"
#include "WorldObject.h"
class Storable:
	virtual public WorldObject {
public:
	Storable();
	virtual ~Storable();
	// Returns a DataObject representing the storable object
	virtual DataObject save();
	// Loads the storable object from the DataObject
	virtual void load(DataObject obj);
};

