#pragma once
#include "BaseState.h"
#include "Button.h"
class BaseMenu:
	public BaseState {
public:
	BaseMenu();
	~BaseMenu();
	// Draws the menu ex seconds after last update
	void draw(double ex);
	// Updates the menu
	void update();
	// Called when a mouse event is fired
	void BaseMenu::mouseEvent(GLFWwindow* window, int button, int action, int mods);
protected:
	vector<Button*> buttons;
};

