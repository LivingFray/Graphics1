#pragma once
#include "Globals.h"
class WorldObject {
public:
	WorldObject();
	~WorldObject();
	// Gets the id of the object
	virtual string getId();
	// Returns the centre of the selectable
	virtual Vec2D getPos();
	// Sets the position of the selectable
	virtual void setPos(Vec2D pos);
	// Gets the angle of the selectable
	virtual double getAngle();
	// Sets the angle of the selectable
	virtual void setAngle(double angle);
	// Gets the width of the selectable
	virtual double getWidth();
	// Sets the width of the selectable
	virtual void setWidth(double width);
	// Gets the height of the selectable
	virtual double getHeight();
	// Sets the width of the selectable
	virtual void setHeight(double height);
protected:
	string id;
	Vec2D pos;
	double angle;
	double width;
	double height;
};

