#pragma once
#include "Collider.h"
#include "Selectable.h"
#include "Storable.h"

class Platform:
	virtual public Collider,
	virtual public Selectable,
	public Storable {
public:
	Platform();
	Platform(const Platform& other);
	virtual ~Platform();
	// Sets the width of the platform
	void setWidth(double width);
	// Sets the height of the platform
	void setHeight(double height);
	// Gets the normal vectors needed to check collision
	virtual Vec2D* getNormals(int* numNormals);
	// Gets the vertices of the bounding box
	virtual Vec2D* getVertices(int* numVertices);
	// Called when a collision occurs
	virtual void onCollide(Collider* other);
	// Draws the platform ex seconds after last update
	virtual void draw(double ex);
	// Returns if the selectable can be moved
	virtual bool canMove();
	// Returns if the selectable can be resized
	virtual bool canResize();
	// Returns if the selectable can be rotated
	virtual bool canRotate();
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
	// Creates an option menu using the current values as defaults
	virtual void createOptions();
	// Updates the platform
	virtual void update();
	// Sets the options for this selectable
	virtual void setOptions(OptionMenu* menu);
protected:
	GLuint texture;
	string textureString;
	double texX;
	double texY;
	double texXSize;
	double texYSize;
};
