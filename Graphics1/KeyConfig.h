#pragma once
#include <map>
#include <string>
#include <GLFW\glfw3.h>

#include "DataObject.h"

using namespace std;
typedef map<string, int> keyMap;

namespace keyconfig {
	extern keyMap keyBindings;
	extern GLFWwindow* win;
	void loadBindings();
	void saveBindings();
	bool isDown(string key);
}