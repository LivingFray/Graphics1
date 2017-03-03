#pragma once
#include "Selectable.h"
class GravityField:
	public Selectable {
public:
	GravityField();
	~GravityField();
	// Draws the gravity field ex seconds after last update
	virtual void draw(double ex);
	// Called when the selectable is moved
	virtual bool onMove(double dX, double dY);
	// Called when the selectable is resized
	virtual bool onResize(double dX, double dY);
	// Called when the selectable is rotated
	virtual bool onRotate(double dAngle);
	// Returns if the selectable can be moved
	virtual bool canMove();
	// Returns if the selectable can be rotated
	virtual bool canResize();
	// Returns if the selectable can be rotated
	virtual bool canRotate();
	// Returns if the selectable is selected
	virtual bool isInBoundingBox(double x, double y);
	// Returns the centre of the selectable
	virtual Vec2D getPos();
	// Sets the position of the selectable
	virtual void setPos(Vec2D pos);
	// Gets the angle of the selectable
	virtual double getAngle();
	// Gets the width of the selectable
	virtual double getWidth();
	// Gets the height of the selectable
	virtual double getHeight();
	// Gets the strength of the gravity field
	virtual double getStrength();
	// Gets the angle of the selectable
	virtual void setAngle(double angle);
	// Gets the width of the selectable
	virtual void setWidth(double width);
	// Gets the height of the selectable
	virtual void setHeight(double height);
	// Sets the strength of the gravity field
	virtual void setStrength(double strength);
protected:
	Vec2D pos;
	double width;
	double height;
	double angle;
	double strength;
};

