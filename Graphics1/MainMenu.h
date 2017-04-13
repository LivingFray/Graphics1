#pragma once
#include "BaseMenu.h"

class MainMenu: public BaseMenu {
public:
	MainMenu();
	~MainMenu();
	// Updates the menu
	void update();
	// Called when a mouse event is fired
	void mouseEvent(GLFWwindow* window, int button, int action, int mods);
	// Called when the window is resized
	void resizeEvent(GLFWwindow* window, int width, int height);
};

