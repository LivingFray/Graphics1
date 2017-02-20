#pragma once
#include "BaseMenu.h"

class MainMenu: public BaseMenu {
public:
	MainMenu();
	~MainMenu();
	// Draws the main menu ex seconds after last update
	void draw(double ex);
	// Updates the menu
	void update();
	// Called when a mouse event is fired
	void MainMenu::mouseEvent(GLFWwindow* window, int button, int action, int mods);
protected:
	GLuint planet;
	GLuint stars;
};

