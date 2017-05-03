#pragma once
#include "Entity.h"
class ButtonEntity :
	public Entity {
public:
	ButtonEntity();
	~ButtonEntity();
	// Called when a collision occurs
	void onCollide(Collider* other);
	// Sets the options for this selectable
	void setOptions(OptionMenu* menu);
	// Creates an option menu using the current values as defaults
	void createOptions();
	// Returns a DataObject representing the storable object
	DataObject save();
	// Loads the storable object from the DataObject
	void load(DataObject obj);
	// Updates the entity
	void update();
protected:
	int channel;
	bool wasDown;
	Animation downAnim;
	Animation upAnim;
};

