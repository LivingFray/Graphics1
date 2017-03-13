#pragma once
#include <vector>
#include "Globals.h"
#include "Platform.h"
#include "Vec2D.h"
#include "Entity.h"
#include "Player.h"
#include "BaseState.h"
#include "LevelRenderer.h"
#include "Button.h"

using namespace std;

class Entity;
class Player;

class Level: 
	public BaseState, 
	public LevelRenderer {
public:
	Level();
	~Level();
	// Called when a mouse event is fired
	void mouseEvent(GLFWwindow * window, int button, int action, int mods);
	// Updates the level
	void update();
	// Loads a level from the given file
	void loadLevel(string filePath);
	// Draws the level
	void draw(double ex);
	// Calculates the force of gravity applied to an object at a location
	void getGravityAtPos(Vec2D pos, Vec2D* grav);
	// Gets the player entity
	Player* getPlayer();
	// Gets the camera position ex seconds after last update
	Vec2D getCameraAt(double ex);
	// Gets the angle of the camera ex seconds after last update
	double getCameraAngleAt(double ex);
	// Loads the next level, provided it can be found
	void nextLevel();
	// Loads the current level from the beginning
	void restartLevel();
protected:
	double defaultGravity = 2; //Fallback for outside the fields
	Player* player;
	GLuint spawnBeam;
	double levelTime;
	// Calculates if two colliders are intersecting and provides the vector to move one in if so
	bool intersects(Collider* a, Collider* b, Vec2D* res);
	// Projects an object onto a vector
	void project(Collider* c, Vec2D vec, double* min, double* max);
	bool reachedGoal;
	int score;
	Button buttonMenu;
	Button buttonRetry;
	Button buttonNext;
};

