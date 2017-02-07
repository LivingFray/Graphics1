#pragma once
#include <string>
#include <vector>
#include "Platform.h"
#include "Vec2D.h"
#include "Entity.h"
#include "Player.h"

using namespace std;

class Entity;
class Player;

class Level {
public:
	Level();
	~Level();
	// Updates the level
	void update();
	// Draws the level
	void draw(double ex);
	// Loads a level from the given file
	void loadLevel(string filePath);
	// Calculates the force of gravity applied to an object at a location
	void getGravityAtPos(Vec2D pos, Vec2D* grav);
	// Gets the player entity
	Player* getPlayer();
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
	// Calculates if two colliders are intersecting and provides the vector to move one in if so
	bool intersects(Collider* a, Collider* b, Vec2D* res);
	// Projects an object onto a vector
	void project(Collider* c, Vec2D vec, double* min, double* max);
};

