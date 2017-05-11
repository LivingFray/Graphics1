#pragma once
#include "Scenery.h"
class Clock :
	public Scenery {
public:
	Clock();
	~Clock();
	// Updates the object
	void update();
	// Draws the timer
	void draw(double ex);
	// Sets the options for this selectable
	void setOptions(OptionMenu * menu);
	// Creates an option menu using the current values as defaults
	void createOptions();
	// Returns a DataObject representing the storable object
	DataObject save();
	// Loads the storable object from the DataObject
	void load(DataObject obj);
	// Returns if the selectable can be resized
	bool canResize();
protected:
	GLuint img;
	int channel;
	double offTime;
	double onTime;
	double time;
	bool on;
};

