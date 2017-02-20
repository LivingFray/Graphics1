#include "Menu.h"
#include "LevelSelect.h"
#include "GradButton.h"

#define MENU_BUTTON_HEIGHT 50

MainMenu::MainMenu() {
	Button* play = new GradButton();
	play->setX(sWidth / 2);
	play->setY(250);
	play->setWidth(400);
	play->setHeight(MENU_BUTTON_HEIGHT);
	play->setLabel("Play");
	auto playCall = []() {
		newState = new LevelSelect();
	};
	play->setCallback(playCall);
	buttons.push_back(play);
	Button* editor = new GradButton();
	editor->setX(sWidth / 2);
	editor->setY(190);
	editor->setWidth(400);
	editor->setHeight(MENU_BUTTON_HEIGHT);
	editor->setLabel("Level Editor");
	auto editorCall = []() {
		printf("Level editor NYI\n");
	};
	editor->setCallback(editorCall);
	buttons.push_back(editor);
	Button* settings = new GradButton();
	settings->setX(sWidth / 2);
	settings->setY(130);
	settings->setWidth(400);
	settings->setHeight(MENU_BUTTON_HEIGHT);
	settings->setLabel("Settings");
	auto settingsCall = []() {
		printf("Clickity\n");
	};
	settings->setCallback(settingsCall);
	buttons.push_back(settings);
	Button* quit = new GradButton();
	quit->setX(sWidth / 2);
	quit->setY(70);
	quit->setWidth(400);
	quit->setHeight(MENU_BUTTON_HEIGHT);
	quit->setLabel("Quit");
	auto quitCall = []() {
		exit(EXIT_SUCCESS);
	};
	quit->setCallback(quitCall);
	buttons.push_back(quit);
	planet = ImageLoader::getImage("Resources\\planet.png");
	stars = ImageLoader::getImage("Resources\\stars.png");
}


MainMenu::~MainMenu() {
}


// Draws the main menu ex seconds after last update
void MainMenu::draw(double ex) {
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
	int i = 0;
	int n = buttons.size();
	for (Button* b : buttons) {
		int h = sHeight / 12;
		b->setWidth((int) sWidth * 0.5 + h * i);
		b->setY(h * (n - i));
		b->setHeight(h*0.9);
		i++;
		b->draw(ex);
	}
}


// Updates the menu
void MainMenu::update() {
}


// Called when a mouse event is fired
void MainMenu::mouseEvent(GLFWwindow* window, int button, int action, int mods) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	y = sHeight - y;
	if (action != GLFW_RELEASE) {
		return;
	}
	for (Button* b : buttons) {
		b->mouseDown((int)x, (int)y);
	}
}
