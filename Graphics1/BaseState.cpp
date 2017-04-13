#include "BaseState.h"

BaseState::BaseState() {
}


BaseState::~BaseState() {
}


// Draws the state
void BaseState::draw(double ex) {
}


// Updates the state
void BaseState::update() {
}


// Called when a key event is fired
void BaseState::keyEvent(GLFWwindow* window, int key, int scan, int action, int mods) {
}


// Called when a mouse event is fired
void BaseState::mouseEvent(GLFWwindow* window, int button, int action, int mods) {
}


// Called when a mousemove event is fired
void BaseState::mouseMoveEvent(GLFWwindow* window, double x, double y) {
}


// Called when a mousemove event is fired
void BaseState::textEvent(GLFWwindow* window, unsigned int ch) {
}


// Called when the window is resized
void BaseState::resizeEvent(GLFWwindow * window, int width, int height) {
}
