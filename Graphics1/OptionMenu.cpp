#include "OptionMenu.h"



OptionMenu::OptionMenu() {
}


OptionMenu::~OptionMenu() {
	for (Option* o : options) {
		delete o;
	}
}


// Adds an option to the option menu
void OptionMenu::addOption(string option, bool numeric, string value) {
	Option* opt = new Option();
	opt->name = option;
	opt->box = TextBox();
	opt->box.setText(value);
	opt->box.setNumeric(numeric);
	options.push_back(opt);
}


// Draws the Option menu
void OptionMenu::draw() {
	bool newline = true;
	float y = sHeight - fontSmall.h * 2;
	//Loop through and draw each option
	//TODO: scrolling?
	for (Option* o : options) {
		//Fit 2 numeric boxes on one line
		newline = newline || !o->box.getNumeric();
		//Update draw position
		if (newline) {
			y -= fontSmall.h * 4;
		}
		float start = sWidth * (newline ? 0.1f : 0.55f);
		float end;
		if (newline && o->box.getNumeric()) {
			end = sWidth * 0.45f;
		} else {
			end = sWidth * 0.9f;
		}
		//Print the label
		glColor3ub(0, 0, 0);
		freetype::print(fontSmall, start, y - fontSmall.h * 0.325f, o->name.c_str());
		start += freetype::getLength(fontSmall, (o->name + " ").c_str());
		//Position the text box
		o->box.setX((int)((start + end) * 0.5f));
		o->box.setY((int)y);
		o->box.setWidth((int)(end - start));
		o->box.setHeight((int)fontSmall.h * 2);
		o->box.draw();
		//Set whether to put the next box on a new line
		newline = !newline || !o->box.getNumeric();
	}
}


// Called when a key is pressed in the menu
void OptionMenu::keyEvent(int key, int scan, int action, int mods) {
	for (Option* o : options) {
		o->box.keyDown(key, scan, action, mods);
	}
}


// Called when text is entered in the menu
void OptionMenu::textEvent(int ch) {
	for (Option* o : options) {
		o->box.textEvent(ch);
	}
}


// Called when the mouse is clicked in the menu
void OptionMenu::mouseEvent(int button, int action, int mods) {
	double x, y;
	glfwGetCursorPos(gameWindow, &x, &y);
	y = sHeight - y;
	if (action == GLFW_RELEASE) {
		for (Option* o : options) {
			o->box.mouseDown(x, y);
		}
	}
}

std::map<string, string> OptionMenu::getValues() {
	std::map<string,string> r;
	for (Option* o : options) {
		r[o->name] = o->box.getText();
	}
	return r;
}
