#pragma once
#include "BaseMenu.h"
class SettingsMenu :
	public BaseMenu {
public:
	SettingsMenu();
	~SettingsMenu();
	// Called when the window is resized
	void resizeEvent(GLFWwindow* window, int width, int height);
	// Called when a key event is fired
	void keyEvent(GLFWwindow* window, int key, int scan, int action, int mods);
};

