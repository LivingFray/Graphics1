#include "BaseMenu.h"



BaseMenu::BaseMenu() {
}


BaseMenu::~BaseMenu() {
	buttons.clear();
}


// Draws the main menu ex seconds after last update
void BaseMenu::draw(double ex) {
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
		b->mouseDown((int)x, (int)y);
	}
}
