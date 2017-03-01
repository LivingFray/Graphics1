#pragma once
#include "Vec2D.h"
#include "Collider.h"
#include "Selectable.h"

class Platform: public Collider, public Selectable {
public:
	Platform();
	~Platform();
	// Gets the position of the platform
	Vec2D getPos();
	// Sets the position of the platform
	void setPos(Vec2D pos);
	// Sets the width of the platform
	void setWidth(double width);
	// Sets the height of the platform
	void setHeight(double height);
	// Gets the width of the platform
	double getWidth();
	// Gets the height of the platform
	double getHeight();
	// Sets the angle of the platform
	void setAngle(double angle);
	// Gets the angle of the platform
	double getAngle();
	// Gets the normal vectors needed to check collision
	virtual Vec2D* getNormals(int* numNormals);
	// Gets the vertices of the bounding box
	virtual Vec2D* getVertices(int* numVertices);
	// Called when a collision occurs
	virtual void onCollide(Collider* other);
	// Draws the platform ex seconds after last update
	virtual void draw(double ex);
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
protected:
	Vec2D pos;
	double width;
	double height;
	double angle;
};
