#pragma once
#include "BaseMenu.h"
#include "GradButton.h"
class LevelSelect:
	public BaseMenu {
public:
	LevelSelect();
	~LevelSelect();
	// Called when the window is resized
	void resizeEvent(GLFWwindow* window, int width, int height);
protected:
	vector<DataObject*> levels;
	GradButton* menuButton;
};

