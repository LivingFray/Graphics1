#pragma once
#include "Vec2D.h"
#include "Collider.h"
#include "Selectable.h"
#include "Storable.h"

class Platform:
	public Collider, public Selectable {
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
	// Returns a DataObject representing the storable object
	virtual DataObject save();
	// Loads the storable object from the DataObject
	virtual void load(DataObject obj);
	// Sets the texture of the platform
	virtual void setTexture(string tex);
	// Sets the scale of the texture in the x axis
	virtual void setTexScaleX(double x);
	// Sets the scale of the texture in the y axis
	virtual void setTexScaleY(double y);
	// Sets the scale of the texture in the x axis
	virtual double getTexScaleX();
	// Sets the scale of the texture in the y axis
	virtual double getTexScaleY();
protected:
	Vec2D pos;
	double width;
	double height;
	double angle;
	GLuint texture;
	string textureString;
	double texX;
	double texY;
	double texXSize;
	double texYSize;
};
