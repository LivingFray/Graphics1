#include "LevelSelect.h"
#include "Level.h"
#include "LevelButton.h"
#include <filesystem>
#include <iostream>
using std::experimental::filesystem::directory_iterator;

LevelSelect::LevelSelect() {
	//Find all the levels we can
	//TODO: Stringent tests on validity of level
	vector<string> paths;
	DATATYPE t;
	for (auto& d : directory_iterator("Levels")) {
		DataObject* lvl = new DataObject();
		lvl->loadFromFile(d.path().string());
		void* dat = lvl->get("name", t);
		if (t == STRING) {
			//TODO: TMP, REMOVE OUTPUT
			std::cout << d << " " << *((string*)dat) << std::endl;
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
		l->setHeight(50);
		l->setLabel(*(string*)levels[i]->get("name", t));
		l->setPath(paths[i]);
		buttons.push_back(l);
	}
	planet = ImageLoader::getImage("Resources\\planet.png");
	stars = ImageLoader::getImage("Resources\\stars.png");
}


LevelSelect::~LevelSelect() {
}


// Draws the main menu ex seconds after last update
void LevelSelect::draw(double ex) {
	//Stars are 1px at 512x512, so scale to make visible
	//TODO: Scale stars in level as well
	float sX = sWidth / 512.0f;
	float sY = sHeight / 512.0f;
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, stars);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(0.0, 0.0);
	glTexCoord2d(sX, 0.0);
	glVertex2d(sWidth, 0.0);
	glTexCoord2d(sX, sY);
	glVertex2d(sWidth, sHeight);
	glTexCoord2d(0.0, sY);
	glVertex2d(0.0, sHeight);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, planet);
	glTranslatef(sWidth * 0.5f, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(-sHeight, 0.0);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(sHeight, 0.0);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(sHeight, sHeight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(-sHeight, sHeight);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//TODO: Resize + reposition buttons
	//TODO: Handle too many levels
	int c = 0;
	for (Button* b : buttons) {
		b->setX((c % 2 == 0 ? 1 : 3)*sWidth / 4);
		c++;
		b->setWidth(sWidth / 2 - 50);
		b->setY(sHeight - ((c + 1) / 2) * 60);
		b->draw();
	}
}