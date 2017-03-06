#pragma once
#include "Globals.h"
#include "Platform.h"
#include "Vec2D.h"
#include "Entity.h"
#include "Player.h"
#include "BaseState.h"
#include "GravityField.h"

class LevelRenderer {
public:
	LevelRenderer();
	~LevelRenderer();
	// Draws the level
	virtual void draw(double ex);
	// Saves the level to the given file
	virtual void saveLevel(string filePath);
	// Loads a level from the given file
	virtual void loadLevel(string filePath);
	// Adds a platform to the level
	virtual void addPlatform(Platform* platform);
	// Adds a gravity field to the level
	virtual void addGravityField(GravityField* field);
	// Gets the camera position ex seconds after last update
	virtual Vec2D getCameraAt(double ex);
	// Gets the angle of the camera ex seconds after last update
	virtual double getCameraAngleAt(double ex);
	// Gets the world coordinates from the screen coordinates
	virtual Vec2D getWorldCoordinates(Vec2D screen);
protected:
	vector<Entity*> entities;
	vector<Platform*> platforms;
	vector<GravityField*> gravFields;
	GLuint planet;
	GLuint stars;
	GLuint backing;
	string levelName;
	double defaultGravity;
	Vec2D spawn;
	double spawnAngle;
	Vec2D goal;
	double goalAngle;
};

