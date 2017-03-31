#pragma once
#include "Platform.h"
class Goal:
	public Platform {
public:
	Goal();
	~Goal();
	// Returns if the selectable can be resized
	bool canResize();
	// Returns if the selectable can be deleted
	bool canDelete();
	// Called when the selectable is moved
	bool onMove(double dx, double dy);
	// Called when the selectable is rotated
	bool onRotate(double da);
	// Draws the goal ex seconds after last update
	void draw(double ex);
	// Creates an option menu using the current values as defaults
	void createOptions();
	// Sets the options for this selectable
	void setOptions(OptionMenu* menu);
	// Returns a DataObject representing the storable object
	DataObject save();
	// Loads the storable object from the DataObject
	void load(DataObject obj);
};

