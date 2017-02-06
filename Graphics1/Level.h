#pragma once
#include <string>
#include <vector>
#include "Entity.h"
#include "Platform.h"
#include "Vec2D.h"

using namespace std;

class Entity;

class Level {
public:
	Level();
	~Level();
	// Updates the level
	void update();
	// Draws the level
	void draw(double ex);
private:
	//A Gravity field always pulls down (towards lowest y-value)
	//Change this by rotating field or inverting the field strength
	struct GravityField {
		Vec2D pos;
		double width;
		double height;
		double rotation;
		double strength;
	};
	vector<Entity*> entities;
	vector<Platform*> platforms;
	vector<GravityField*> gravFields;
	double defaultGravity = 2; //Fallback for outside the fields
public:
	// Loads a level from the given file
	void loadLevel(string filePath);
	// Calculates the force of gravity applied to an object at a location
	void getGravityAtPos(Vec2D pos, Vec2D* grav);
};

