#include "LevelSelect.h"
#include "Level.h"
#include "LevelButton.h"
#include "MainMenu.h"
#include <filesystem>
#include <iostream>
using std::experimental::filesystem::directory_iterator;

LevelSelect::LevelSelect() {
	//Find all the levels we can
	//TODO: Stringent tests on validity of level
	vector<string> paths;
	bool exists;
	for (auto& d : directory_iterator("Levels")) {
		DataObject* lvl = new DataObject();
		lvl->loadFromFile(d.path().string());
		string lName = lvl->getString("name", exists);
		if (exists) {
			levels.push_back(lvl);
			paths.push_back(d.path().string());
		}
	}
	//TODO: Scaling
	for (unsigned int i = 0; i < levels.size(); i++) {
		LevelButton* l = new LevelButton();
		l->setX((i % 2 == 0 ? 1 : 3)*sWidth / 4);
		l->setY(sHeight - (1 + (i / 2)) * 50);
		l->setWidth(sWidth / 4);
		l->setHeight((int)fontLarge.h * 2);
		l->setLabel(levels[i]->getString("name"));
		l->setPath(paths[i]);
		buttons.push_back(l);
	}
	planet = ImageLoader::getImage("Resources\\planet.png");
	stars = ImageLoader::getImage("Resources\\stars.png");
	menuButton = new GradButton();
	menuButton->setLabel("Return to menu");
	auto menuCall = [](BaseState* s) {
		newState = new MainMenu();
	};
	menuButton->setCallback(menuCall);
	buttons.push_back(menuButton);
}


LevelSelect::~LevelSelect() {
	//TODO: Delete buttons
}


// Called when the window is resized
void LevelSelect::resizeEvent(GLFWwindow* window, int width, int height) {
	//TODO: Handle too many levels
	int c = 0;
	for (Button* b : buttons) {
		b->setX((c % 2 == 0 ? 1 : 3)*sWidth / 4);
		c++;
		b->setWidth(sWidth / 2 - 50);
		b->setHeight((int)fontLarge.h * 2);
		b->setY(sHeight - ((c + 1) / 2) * (int)(fontLarge.h * 2.1));
	}
	menuButton->setWidth(sWidth / 2);
	menuButton->setHeight((int)fontLarge.h * 2);
	menuButton->setY((int)fontLarge.h);
}