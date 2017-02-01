#pragma once
#include <map>
#include <string>
#include <GLFW\glfw3.h>

#include "DataObject.h"

using namespace std;
typedef map<string, int> keyMap;

namespace keyconfig {
	extern keyMap keyBindings;

	void loadBindings();
	void saveBindings();
	int get(string key);
}