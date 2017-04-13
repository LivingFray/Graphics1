#pragma once
#include "BaseState.h"
#include "Button.h"
class BaseMenu:
	public BaseState {
public:
	BaseMenu();
	~BaseMenu();
	// Draws the menu ex seconds after last update
	virtual void draw(double ex);
	// Updates the menu
	virtual void update();
	// Called when a mouse event is fired
	virtual void mouseEvent(GLFWwindow* window, int button, int action, int mods);
protected:
	vector<Button*> buttons;
	GLuint planet;
	GLuint stars;
};

