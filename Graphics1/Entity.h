#pragma once
#include "Vec2D.h"
#include "Collider.h"
#include "Selectable.h"
#include "Storable.h"
#include "Animation.h"

using namespace std;

class Entity:
	public Collider,
	public Selectable,
	public Storable {
public:
	Entity();
	~Entity();
	// Gets the x-coordinate of the entity
	double getX();
	// Gets the y-coordinate of the entity
	double getY();
	// Sets the x-coordinate of the entity
	void setX(double x);
	// Sets the y-coordinate of the entity
	void setY(double y);
	// Updates the entity
	virtual void update();
	// Draws the entity
	virtual void draw(double ex);
	// Adds the given value to the horizontal velocity
	void addVelX(double x);
	// Adds the given vaue to the vertical velocity
	void addVelY(double y);
	// Adds the value to the horizontal position
	void addPosX(double x);
	// Adds the value to the vertical position
	void addPosY(double y);
	// Sets the entity's max speed
	void setMaxSpeed(double speed);
	// Gets the entity's max speed
	double getMaxSpeed();
	// Gets the normal vectors needed to check collision
	virtual Vec2D* getNormals(int* numNormals);
	// Gets the vertices of the bounding box
	virtual Vec2D* getVertices(int* numVertices);
	// Sets whether the entity is currently on the ground
	void setOnGround(bool onGround);
	// Gets if the entity is currently on the ground
	bool getOnGround();
	// Gets the angle the entity seen at
	double getVisAngle();
	// Sets the angle the entity is seen at
	void setVisAngle(double angle);
	// Gets the updated visible angle for ex seconds of motion
	double updatedVisAngle(double ex);
	// Returns if the selectable can be moved
	virtual bool canMove();
	// Returns if the selectable can be rotated
	virtual bool canResize();
	// Returns if the selectable can be rotated
	virtual bool canRotate();
	// Returns whether the entity is currently moving
	bool isMoving();
protected:
	double visAngle;
	bool onGround;
	double maxSpeed;
	bool flip = false;
	bool moving;
	Animation currentAnim;
};

