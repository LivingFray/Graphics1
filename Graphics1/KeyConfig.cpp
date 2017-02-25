#include "KeyConfig.h"

#define bindControl(n,k)	do{												\
								keyBindings[(n)] = (k);						\
								void* stored = bindings->get((n), datatype);\
								if (datatype == INT) {						\
									keyBindings[(n)] = *(int*)stored;		\
								}else{										\
									int v = (k);							\
									bindings->add((n),INT,&v);				\
								}											\
							} while(0)

keyMap KeyConfig::keyBindings;
GLFWwindow* KeyConfig::win;

// Loads default keybindings then sets custom bindings where available
void KeyConfig::loadBindings() {
	DataObject* bindings = new DataObject();
	bindings->loadFromFile("keybindings.do");
	DATATYPE datatype;
	//By the power of macros, check if each key is bound and rebind if possible
	bindControl("moveLeft", GLFW_KEY_LEFT);
	bindControl("moveRight", GLFW_KEY_RIGHT);
	bindControl("jump", GLFW_KEY_UP);
	bindControl("editorLeft", GLFW_KEY_LEFT);
	bindControl("editorRight", GLFW_KEY_RIGHT);
	bindControl("editorUp", GLFW_KEY_UP);
	bindControl("editorDown", GLFW_KEY_DOWN);
	bindControl("editorMenu", GLFW_KEY_ENTER);
	//Save the keybindings (some newer default bindings may have been missing)
	bindings->saveToFile("keybindings.do");
	delete bindings;
}

void KeyConfig::saveBindings() {
	DataObject* bindings = new DataObject();

	for (auto const& key : keyBindings) {
		int v = key.second;
		bindings->add(key.first, INT, &v);
	}
	bindings->saveToFile("keybindings.do");
	delete bindings;
}

bool KeyConfig::isDown(string key) {
	return glfwGetKey(win, keyBindings[key]) == GLFW_PRESS;
}