#pragma once
#include "BaseState.h"
#include "Button.h"

class Menu: public BaseState {
public:
	Menu();
	~Menu();
	// Draws the main menu ex seconds after last update
	void draw(double ex);
	// Updates the menu
	void update();
	// Called when a mouse event is fired
	void Menu::mouseEvent(GLFWwindow* window, int button, int action, int mods);
protected:
	GLuint planet;
	GLuint stars;
	vector<Button*> buttons;
};

