#pragma once
#include "Scenery.h"
class Light :
	public Scenery {
public:
	Light();
	~Light();
	// Draws the logic gate
	void draw(double ex);
	// Sets the options for this selectable
	void setOptions(OptionMenu * menu);
	// Creates an option menu using the current values as defaults
	void createOptions();
	// Returns a DataObject representing the storable object
	DataObject save();
	// Loads the storable object from the DataObject
	void load(DataObject obj);
	// Whether the object can be resized
	bool canResize();
protected:
	int channel;
	GLuint imgOff, imgOn;
};

