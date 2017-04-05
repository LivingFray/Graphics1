#pragma once
#include "Globals.h"

typedef map<string, int> keyMap;

namespace KeyConfig {
	extern keyMap keyBindings;
	extern GLFWwindow* win;
	void loadBindings();
	void saveBindings();
	bool isDown(string key);
}