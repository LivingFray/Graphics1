#pragma once
#include "Globals.h"

typedef map<string, int> keyMap;
typedef map<string, string> nameMap;

namespace KeyConfig {
	extern keyMap keyBindings;
	extern nameMap bindingNames;
	extern GLFWwindow* win;
	// Loads default keybindings then sets custom bindings where available
	void loadBindings();
	// Saves the key bindings to file
	void saveBindings();
	// Returns if a bound key is down
	bool isDown(string key);
	// Binds a key to a binding
	void bind(string binding, int key);
	// Returns a list of bindings
	string* getBindings(int *size);
	// Gets the user friendly name of a binding
	string getFriendlyName(string binding);
	// Gets the key bound to a binding
	int getKey(string binding);
	// Gets the human readbale name of a key
	string getReadable(int key);
}