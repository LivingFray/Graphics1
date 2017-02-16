#pragma once
#include "BaseMenu.h"
class LevelSelect:
	public BaseMenu {
public:
	LevelSelect();
	~LevelSelect();
	// Draws the main menu ex seconds after last update
	void LevelSelect::draw(double ex);
protected:
	vector<DataObject*> levels;
	GLuint planet = ImageLoader::getImage("Resources\\planet.png");
	GLuint stars = ImageLoader::getImage("Resources\\stars.png");
};

