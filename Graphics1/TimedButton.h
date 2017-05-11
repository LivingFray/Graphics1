#pragma once
#include "Lever.h"
class TimedButton :
	public Lever {
public:
	TimedButton();
	~TimedButton();
	// Updates the object
	void update();
	// Sets the options for this selectable
	void setOptions(OptionMenu* menu);
	// Creates an option menu using the current values as defaults
	void createOptions();
	// Returns a DataObject representing the storable object
	DataObject save();
	// Loads the storable object from the DataObject
	void load(DataObject obj);
protected:
	double activeTime;
	double cooldown;
	ALuint offSound;
};

