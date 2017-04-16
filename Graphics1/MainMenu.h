#pragma once
#include "BaseMenu.h"

class MainMenu: public BaseMenu {
public:
	MainMenu();
	~MainMenu();
	// Updates the menu
	void update();
	// Called when the window is resized
	void resizeEvent(GLFWwindow* window, int width, int height);
};

