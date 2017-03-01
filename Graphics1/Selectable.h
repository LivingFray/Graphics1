#pragma once
#include "Vec2D.h"
class Selectable {
public:
	Selectable();
	~Selectable();
	// Called when the selectable is moved
	virtual void onMove(double dX, double dY);
	// Called when the selectable is resized
	virtual void onResize(double dX, double dY);
	// Called when the selectable is rotated
	virtual void onRotate(double dAngle);
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
};

