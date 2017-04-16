#include "LevelButton.h"
#include "Level.h"


LevelButton::LevelButton() {
}


LevelButton::~LevelButton() {
}


// Sets the path of the level to be loaded when it is clicked
void LevelButton::setPath(string path) {
	this->path = path;
}


// Gets the path of the level to be loaded when it is clicked
string LevelButton::getPath() {
	return path;
}

// Called when the left mouse button is clicked
bool LevelButton::mouseDown(int x, int y) {
	float w = width*0.5f;
	float h = height*0.5f;
	if (x >= this->x - w && x <= this->x + w && y >= this->y - h && y <= this->y + h) {
		newState = new Level();
		((Level*)newState)->loadLevel(path);
		return true;
	}
	return false;
}
