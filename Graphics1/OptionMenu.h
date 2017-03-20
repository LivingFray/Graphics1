#pragma once
#include "Globals.h"
#include "TextBox.h"
class OptionMenu {
public:
	OptionMenu();
	~OptionMenu();
	// Adds an option to the option menu
	void addOption(string option, bool numeric, string value = "");
	// Draws the Option menu
	void draw();
	// Called when a key is pressed in the menu
	void keyEvent(int key, int scan, int action, int mods);
	// Called when text is entered in the menu
	void textEvent(int ch);
	// Called when the mouse is clicked in the menu
	void mouseEvent(int button, int action, int mods);
	// Gets the values associated with the options
	std::map<string, string> getValues();
private:
	struct Option {
		string name;
		TextBox box;
	};
	vector<Option*> options;
};

