#include "BaseMenu.h"



BaseMenu::BaseMenu() {
	planet = ImageLoader::getImage("Resources\\planet.png");
	stars = ImageLoader::getImage("Resources\\stars.png");
}


BaseMenu::~BaseMenu() {
	for (Button* b : buttons) {
		delete b;
	}
	buttons.clear();
}


// Draws the main menu ex seconds after last update
void BaseMenu::draw(double ex) {
	//Stars are 1px at 512x512, so scale to make visible
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
	for (Button* b : buttons) {
		b->draw(ex);
	}
}


// Updates the menu
void BaseMenu::update() {
}


// Called when a mouse event is fired
void BaseMenu::mouseEvent(GLFWwindow* window, int button, int action, int mods) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	y = sHeight - y;
	if (action != GLFW_RELEASE) {
		return;
	}
	for (Button* b : buttons) {
		if (b->mouseDown((int)x, (int)y)) {
			break;
		}
	}
}
