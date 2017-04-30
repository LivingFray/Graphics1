#pragma once
#include "Interactive.h"
class Lever :
	public Interactive {
public:
	Lever();
	~Lever();
	// Updates the entity
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
	Animation sOffAnim;
	Animation nOffAnim;
	Animation sOnAnim;
	Animation nOnAnim;
	bool selected;
	bool active;
	bool wasDown;
	int channel;
	ALuint clickSound;
};

