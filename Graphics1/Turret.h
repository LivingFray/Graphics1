#pragma once
#include "StompableEntity.h"
class Turret :
	public StompableEntity {
public:
	Turret();
	Turret(const Turret& other);
	~Turret();
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
	double cooldown;
	double maxRange;
	double maxCooldown;
};

