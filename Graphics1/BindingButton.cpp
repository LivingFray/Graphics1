#include "BindingButton.h"



BindingButton::BindingButton() {
	friendlyName = "UNKNOWN_BINDING";
	binding = "UNKNOWN_BINDING";
	key = 0;
	font = &fontSmall;
}


BindingButton::~BindingButton() {
}


// Called when a key is pressed
void BindingButton::keyEvent(GLFWwindow* window, int key, int scan, int action, int mods) {
	if (readingInput) {
		this->key = key;
		readingInput = false;
		KeyConfig::bind(binding, key);
		label = friendlyName + ": " + KeyConfig::getReadable(key);;
	}
}


// Called when the left mouse button is clicked
bool BindingButton::mouseDown(int x, int y) {
	float w = width*0.5f;
	float h = height*0.5f;
	if (x >= this->x - w && x <= this->x + w && y >= this->y - h && y <= this->y + h) {
		if (readingInput) {
			readingInput = false;
			label = friendlyName + ": " + KeyConfig::getReadable(key);
		} else {
			readingInput = true;
			label = friendlyName + ": ???";
		}
		return true;
	}
	return false;
}


// Sets the name of the key binding
void BindingButton::setBinding(string binding) {
	this->binding = binding;
	key = KeyConfig::getKey(binding);
}


// Sets the user friendly name of the key binding
void BindingButton::setFriendlyName(string name) {
	friendlyName = name;
	label = friendlyName + ": " + KeyConfig::getReadable(key);
}
