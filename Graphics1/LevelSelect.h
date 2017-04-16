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
	// Changes to the next page of levels
	void next();
	// Changes to the previous page of levels
	void prev();
	// Draws the menu
	void draw(double ex);
protected:
	vector<Button*> allButtons;
	vector<DataObject*> levels;
	GradButton* menuButton;
	GradButton* prevButton;
	GradButton* nextButton;
	unsigned int page;
	unsigned int numPages;
	void updateButtons();
};

