#pragma once
#include "Vec2D.h"

class Platform {
public:
	Platform();
	~Platform();
protected:
	Vec2D pos;
	double width;
	double height;
	double angle;
public:
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
};
