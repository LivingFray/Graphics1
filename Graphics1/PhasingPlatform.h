#pragma once
#include "Platform.h"
class PhasingPlatform :
	public Platform {
public:
	PhasingPlatform();
	~PhasingPlatform();
	bool isSolid();
	// Creates an option menu using the current values as defaults
	virtual void createOptions();
	// Draws the platform ex seconds after last update
	void draw(double ex);
	// Updates the list of supported entities
	void updateSupporting();
	// Sets the options for this selectable
	virtual void setOptions(OptionMenu* menu);
	// Returns a DataObject representing the storable object
	virtual DataObject save();
	// Loads the storable object from the DataObject
	virtual void load(DataObject obj);
protected:
	int channel;
};

