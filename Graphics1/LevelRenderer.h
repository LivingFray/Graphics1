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
	// Gets the camera position ex seconds after last update
	virtual Vec2D getCameraAt(double ex);
	// Gets the angle of the camera ex seconds after last update
	virtual double getCameraAngleAt(double ex);
	// Gets the world coordinates from the screen coordinates
	virtual Vec2D getWorldCoordinates(Vec2D screen);
	// Sets the position of the spawn point
	virtual void setSpawn(Vec2D pos);
	// Gets the position of the spawn point
	virtual Vec2D getSpawn();
	// Sets the position of the goal
	virtual void setGoal(Vec2D pos);
	// Gets the position of the goal
	virtual Vec2D getGoal();
	// Sets the angle of the spawn point
	virtual void setSpawnAngle(double a);
	// Gets the angle of the spawn point
	virtual double getSpawnAngle();
	// Sets the angle of the goal
	virtual void setGoalAngle(double a);
	// Gets the angle of the goal
	virtual double getGoalAngle();
protected:
	vector<Entity*> entities;
	vector<Platform*> platforms;
	vector<GravityField*> gravFields;
	GLuint planet;
	GLuint stars;
	GLuint backing;
	GLuint spawnBack;
	string levelName;
	string nextLevelPath;
	string levelPath;
	double defaultGravity;
	Vec2D spawn;
	double spawnAngle;
	Vec2D goal;
	double goalAngle;
public:
	// Adds an entity to the level
	void addEntity(Entity* entity);
	// Removes the entity from the level if it exists
	void removeEntity(Entity* entity);
};

