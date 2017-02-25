#pragma once
#include "Globals.h"
class BaseState {
public:
	BaseState();
	~BaseState();
	// Draws the state
	virtual void draw(double ex);
	// Updates the state
	virtual void update();
	// Called when a key event is fired
	virtual void keyEvent(GLFWwindow* window, int key, int scan, int action, int mods);
	// Called when a mouse event is fired
	virtual void mouseEvent(GLFWwindow* window, int button, int action, int mods);
	// Called when a mousemove event is fired
	virtual void mouseMoveEvent(GLFWwindow* window, double x, double y);
};

