#include "LevelSelect.h"
#include "Level.h"
#include "LevelButton.h"
#include "MainMenu.h"
#include <filesystem>
#include <iostream>

#define BUTTONS_PER_PAGE 14
using std::experimental::filesystem::directory_iterator;

LevelSelect::LevelSelect() {
	//Find all levels in Level folder
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
	//Add button for each level
	for (unsigned int i = 0; i < levels.size(); i++) {
		LevelButton* l = new LevelButton();
		l->setLabel(levels[i]->getString("name"));
		l->setPath(paths[i]);
		allButtons.push_back(l);
		if (i < BUTTONS_PER_PAGE) {
			buttons.push_back(l);
		}
	}
	page = 0;
	numPages = (levels.size()-1) / BUTTONS_PER_PAGE + 1;
	if (numPages == 0) {
		numPages = 1;
	}
	menuButton = new GradButton();
	menuButton->setLabel("Return to menu");
	auto menuCall = [](BaseState* s) {
		newState = new MainMenu();
	};
	menuButton->setCallback(menuCall);
	buttons.push_back(menuButton);
	nextButton = new GradButton();
	nextButton->setLabel("Next Page");
	auto nextCall = [](BaseState* s) {
		LevelSelect* l = (LevelSelect*)s;
		l->next();
	};
	nextButton->setCallback(nextCall);
	buttons.push_back(nextButton);
	prevButton = new GradButton();
	prevButton->setLabel("Previous Page");
	auto prevCall = [](BaseState* s) {
		LevelSelect* l = (LevelSelect*)s;
		l->prev();
	};
	prevButton->setCallback(prevCall);
	buttons.push_back(prevButton);
}


LevelSelect::~LevelSelect() {
	delete menuButton;
	delete nextButton;
	delete prevButton;
	//Prevent accidental double freeing
	buttons.clear();
	for (Button* b : allButtons) {
		delete b;
	}
}


// Called when the window is resized
void LevelSelect::resizeEvent(GLFWwindow* window, int width, int height) {
	int c = 0;
	for (Button* b : buttons) {
		b->setX((c % 2 == 0 ? 1 : 3)*sWidth / 4);
		c++;
		b->setWidth(sWidth / 2 - 50);
		b->setHeight((int)fontLarge.h * 2);
		b->setY(sHeight - ((c + 3) / 2) * (int)(fontLarge.h * 2.1f));
	}
	menuButton->setWidth(sWidth / 2);
	menuButton->setHeight((int)(fontLarge.h * 1.9f));
	menuButton->setY((int)fontLarge.h);
	nextButton->setWidth(sWidth / 2 - (int)fontLarge.h * 4);
	nextButton->setHeight((int)(fontLarge.h * 1.9f));
	nextButton->setY((int)fontLarge.h * 5);
	prevButton->setWidth(sWidth / 2 - (int)fontLarge.h * 2);
	prevButton->setHeight((int)(fontLarge.h * 1.9f));
	prevButton->setY((int)fontLarge.h * 3);
}


// Changes to the next page of levels
void LevelSelect::next() {
	page = (page + 1) % numPages;
	updateButtons();
}


// Changes to the previous page of levels
void LevelSelect::prev() {
	page--;
	if (page == -1) {
		page = numPages - 1;
	}
	updateButtons();
}


// Draws the main menu ex seconds after last update
void LevelSelect::draw(double ex) {
	BaseMenu::draw(ex);
	string text = "Level Select, Page " + to_string(page + 1) + "/" + to_string(numPages);
	printCentre(fontLarge, sWidth / 2, sHeight - fontLarge.h * 1.375f, text.c_str());
}


void LevelSelect::updateButtons() {
	buttons.clear();
	for (unsigned int i = page * BUTTONS_PER_PAGE; i < (page + 1) * BUTTONS_PER_PAGE && i < allButtons.size(); i++) {
		buttons.push_back(allButtons[i]);
	}
	buttons.push_back(menuButton);
	buttons.push_back(nextButton);
	buttons.push_back(prevButton);
	resizeEvent(gameWindow, sWidth, sHeight);
}
