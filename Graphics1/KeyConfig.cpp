#include "KeyConfig.h"

#define bindControl(n,k)	do{												\
								keyBindings[(n)] = (k);						\
								int st = bindings.getInt((n), exists);		\
								if (exists) {								\
									keyBindings[(n)] = st;					\
								}else{										\
									bindings.add((n), (k));					\
								}											\
							} while(0)

keyMap KeyConfig::keyBindings;
GLFWwindow* KeyConfig::win;

// Loads default keybindings then sets custom bindings where available
void KeyConfig::loadBindings() {
	DataObject bindings = DataObject();
	bindings.loadFromFile("keybindings.do");
	bool exists;
	//By the power of macros, check if each key is bound and rebind if possible
	bindControl("moveLeft", GLFW_KEY_LEFT);
	bindControl("moveRight", GLFW_KEY_RIGHT);
	bindControl("jump", GLFW_KEY_UP);
	bindControl("editorLeft", GLFW_KEY_LEFT);
	bindControl("editorRight", GLFW_KEY_RIGHT);
	bindControl("editorUp", GLFW_KEY_UP);
	bindControl("editorDown", GLFW_KEY_DOWN);
	bindControl("editorAntiClockwise", GLFW_KEY_RIGHT_CONTROL);
	bindControl("editorClockwise", GLFW_KEY_RIGHT_SHIFT);
	bindControl("editorMenu", GLFW_KEY_ENTER);
	//Save the keybindings (some newer default bindings may have been missing)
	bindings.saveToFile("keybindings.do");
}

void KeyConfig::saveBindings() {
	DataObject bindings = DataObject();

	for (auto const& key : keyBindings) {
		bindings.add(key.first, key.second);
	}
	bindings.saveToFile("keybindings.do");
}

bool KeyConfig::isDown(string key) {
	return glfwGetKey(win, keyBindings[key]) == GLFW_PRESS;
}