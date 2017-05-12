#pragma once
#include "Platform.h"
class MovingPlatform :
	public Platform {
public:
	MovingPlatform();
	~MovingPlatform();
	// Returns a DataObject representing the storable object
	virtual DataObject save();
	// Loads the storable object from the DataObject
	virtual void load(DataObject obj);
	// Sets the first node the platform travels to
	void setFirstNode(Vec2D node);
	// Sets the second node the platform travels to
	void setLastNode(Vec2D node);
	// Gets the first node the platfrom travels to
	Vec2D getFirstNode();
	// Gets the last node the platform travels to
	Vec2D getLastNode();
	// Sets the travel time from one node to the other
	void setTravelTime(double t);
	// Gets the travel time from one node to the other
	double getTravelTime();
	// Creates an option menu using the current values as defaults
	virtual void createOptions();
	// Updates the platform
	virtual void update();
	// Called when the selectable is moved
	bool onMove(double dX, double dY);
	// Draws the platform
	void draw(double ex);
	// Sets the options for this selectable
	virtual void setOptions(OptionMenu* menu);
protected:
	Vec2D nodes[2];
	char currentNode;
	double progress;
	double travelTime;
	int channel;
	Vec2D cVel;
};

