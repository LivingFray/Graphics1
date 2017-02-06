#pragma once
#include "Level.h"
#include "Vec2D.h"
using namespace std;

class Level;

class Entity {
public:
	Entity();
	~Entity();
protected:
	Vec2D pos;
	Vec2D vel;
	double angle;
	Level* level;
public:
	// Gets the x-coordinate of the entity
	double getX();
	// Gets the y-coordinate of the entity
	double getY();
	// Sets the x-coordinate of the entity
	void setX(double x);
	// Sets the y-coordinate of the entity
	void setY(double y);
	// Gets the horizontal velocity of the entity
	double getVelX();
	// Gets the vertical velocity of the entity
	double getVelY();
	// Sets the horizontal velocity of the entity
	void setVelX(double x);
	// Sets the vertical velocity of the entity
	void setVelY(double x);
	// Updates the entity
	virtual void update();
	// Draws the entity
	virtual void draw();
	// Adds the given value to the horizontal velocity
	void addVelX(double x);
	// Adds the given vaue to the vertical velocity
	void addVelY(double y);
	// Adds the value to the horizontal position
	void addPosX(double x);
	// Adds the value to the vertical position
	void addPosY(double y);
protected:
	double maxSpeed;
public:
	// Sets the entity's max speed
	void setMaxSpeed(double speed);
	// Gets the entity's max speed
	double getMaxSpeed();
	// Gets the horizontal (relative to the rotation) velocity
	double getVelRelX(double theta);
	// Gets the vertical (relative to the rotation) velocity
	double getVelRelY(double theta);
	// Sets the level in which the entity exists
	void setLevel(Level* level);
	// Gets the level in which the entity exists
	Level* getLevel();
	// Gets the angle of the entity
	double getAngle();
	// Sets the angle of the entity
	void setAngle(double angle);
};

