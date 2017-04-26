#pragma once
#include "WorldObject.h"
#include "OptionMenu.h"
class Selectable:
	virtual public WorldObject {
public:
	Selectable();
	virtual ~Selectable();
	// Called when the selectable is moved
	virtual bool onMove(double dX, double dY);
	// Called when the selectable is resized
	virtual bool onResize(double dX, double dY);
	// Called when the selectable is rotated
	virtual bool onRotate(double dAngle);
	// Returns if the selectable can be deleted
	virtual bool canDelete();
	// Returns if the selectable is selected
	virtual bool isInBoundingBox(double x, double y);
	// Creates an option menu using the current values as defaults
	virtual void createOptions();
	// Gets the options screen for this selectable
	virtual OptionMenu* getOptions();
	// Sets the options for this selectable
	virtual void setOptions(OptionMenu* menu);
protected:
	OptionMenu* options;
};

