#pragma once
#include "Scenery.h"
class LogicGate :
	public Scenery {
public:
	LogicGate();
	virtual ~LogicGate();
	// Draws the logic gate
	void draw(double ex);
	// Sets the options for this selectable
	virtual void setOptions(OptionMenu* menu);
	// Creates an option menu using the current values as defaults
	virtual void createOptions();
	// Returns a DataObject representing the storable object
	virtual DataObject save();
	// Loads the storable object from the DataObject
	virtual void load(DataObject obj);
	// Returns if the selectable can be rotated
	bool canResize();
protected:
	int inChannelA;
	int inChannelB;
	int outChannel;
	bool visible;
	GLuint img;
};

