#include "MainMenu.h"
#include "LevelSelect.h"
#include "LevelEditor.h"
#include "GradButton.h"

#define MENU_BUTTON_HEIGHT 50

MainMenu::MainMenu() {
	Button* play = new GradButton();
	play->setLabel("Play");
	auto playCall = [](BaseState* s) {
		newState = new LevelSelect();
	};
	play->setCallback(playCall);
	buttons.push_back(play);
	Button* editor = new GradButton();
	editor->setLabel("Level Editor");
	auto editorCall = [](BaseState* s) {
		newState = new LevelEditor();
	};
	editor->setCallback(editorCall);
	buttons.push_back(editor);
	Button* settings = new GradButton();
	settings->setLabel("Settings");
	auto settingsCall = [](BaseState* s) {
		printf("Clickity\n");
	};
	settings->setCallback(settingsCall);
	buttons.push_back(settings);
	Button* quit = new GradButton();
	quit->setLabel("Quit");
	auto quitCall = [](BaseState* s) {
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
		int h = (int)fontLarge.h * 2;
		b->setWidth((int)(sWidth * 0.5 + h * i));
		b->setY(h * (n - i));
		b->setHeight((int)(h*0.9));
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
