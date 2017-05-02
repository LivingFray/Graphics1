#pragma once
#include "Selectable.h"
#include "Storable.h"
#include "ParticleSystem.h"

class GravityField:
	public Selectable,
	public Storable {
public:
	GravityField();
	~GravityField();
	// Draws the gravity field ex seconds after last update
	virtual void draw(double ex);
	// Updates the grabity field
	virtual void update();
	// Gets the strength of the gravity field
	virtual double getStrength();
	// Sets the strength of the gravity field
	virtual void setStrength(double strength);
	// Returns a DataObject representing the storable object
	virtual DataObject save();
	// Loads the storable object from the DataObject
	virtual void load(DataObject obj);
	// Creates an option menu using the current values as defaults
	virtual void createOptions();
	// Called when the selectable is resized
	bool onResize(double dX, double dY);
	// Sets the options for this selectable
	virtual void setOptions(OptionMenu* menu);
protected:
	double strength;
	double time;
	double lastTime;
	ParticleSystem p;
	bool warmed;
};

