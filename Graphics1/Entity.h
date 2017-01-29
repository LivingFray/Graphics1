#pragma once
class Entity {
public:
	Entity();
	~Entity();
protected:
	double posX;
	double posY;
	double velX;
	double velY;
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
	void setVelX(double velX);
	// Sets the vertical velocity of the entity
	void setVelY(double velY);
	// Updates the entity
	virtual void update();
	// Draws the entity
	virtual void draw();
};

