#pragma once
#include "Globals.h"
#include "Platform.h"
#include "Vec2D.h"
#include "Entity.h"
#include "Player.h"
#include "BaseState.h"

class LevelRenderer {
public:
	LevelRenderer();
	~LevelRenderer();
	// Draws the level
	virtual void draw(double ex);
	// Loads a level from the given file
	virtual void loadLevel(string filePath);
	// Adds a platform to the level
	virtual void addPlatform(Platform* platform);
	// Gets the camera position ex seconds after last update
	virtual Vec2D getCameraAt(double ex);
	// Gets the angle of the camera ex seconds after last update
	virtual double getCameraAngleAt(double ex);
	// Gets the world coordinates from the screen coordinates
	virtual Vec2D getWorldCoordinates(Vec2D screen);
protected:
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
	//Background
	GLuint planet;
	GLuint stars;
};

