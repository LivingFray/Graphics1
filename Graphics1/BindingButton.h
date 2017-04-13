#pragma once
#include "Button.h"
class BindingButton :
	public Button {
public:
	BindingButton();
	~BindingButton();
	// Called when a key is pressed
	void keyEvent(GLFWwindow* window, int key, int scan, int action, int mods);
	// Called when the left mouse button is clicked
	void mouseDown(int x, int y);
	// Sets the name of the key binding
	void setBinding(string binding);
	// Sets the user friendly name of the key binding
	void setFriendlyName(string name);
protected:
	string binding;
	int key;
	bool readingInput;
	string friendlyName;
};

