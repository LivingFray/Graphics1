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
	// Gets the horizontal velocity of the entity
	double getVelX();
	// Gets the vertical velocity of the entity
	double getVelY();
	// Gets the current velocity of the entity
	Vec2D getVel();
	// Sets the horizontal velocity of the entity
	void setVelX(double x);
	// Sets the vertical velocity of the entity
	void setVelY(double x);
	// Sets the current velocity of the entity
	void setVel(Vec2D vel);
	// Gets the horizontal (relative to the rotation) velocity
	double getVelRelX(double theta);
	// Gets the vertical (relative to the rotation) velocity
	double getVelRelY(double theta);
	// Returns if the world object can be moved
	virtual bool canMove();
	// Returns if the world object can be rotated
	virtual bool canResize();
	// Returns if the world object can be rotated
	virtual bool canRotate();
	// Called when damage is inflicted on the object
	virtual void onDamage(Damage d);
protected:
	string id;
	Vec2D pos;
	Vec2D vel;
	double angle;
	double width;
	double height;
};

