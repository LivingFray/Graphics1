#pragma once
#include "Button.h"
class LevelButton:
	public Button {
public:
	LevelButton();
	virtual ~LevelButton();
protected:
	string path;
public:
	// Sets the path of the level to be loaded when it is clicked
	void setPath(string path);
	// Gets the path of the level to be loaded when it is clicked
	string getPath();
	// Called when the left mouse button is clicked
	bool mouseDown(int x, int y);
};

