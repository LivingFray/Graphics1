#include "KeyConfig.h"

#define bindControl(n,k,f)	do{												\
								keyBindings[(n)] = (k);						\
								bindingNames[(n)] = (f);					\
								int st = bindings.getInt((n), exists);		\
								if (exists) {								\
									keyBindings[(n)] = st;					\
								}else{										\
									bindings.add((n), (k));					\
								}											\
							} while(0)

keyMap KeyConfig::keyBindings;
nameMap KeyConfig::bindingNames;
GLFWwindow* KeyConfig::win;

// Loads default keybindings then sets custom bindings where available
void KeyConfig::loadBindings() {
	DataObject bindings = DataObject();
	bindings.loadFromFile("keybindings.do");
	bool exists;
	//By the power of macros, check if each key is bound and rebind if possible
	bindControl("moveLeft", GLFW_KEY_LEFT, "Move left");
	bindControl("moveRight", GLFW_KEY_RIGHT, "Move right");
	bindControl("jump", GLFW_KEY_UP, "Jump");
	bindControl("editorLeft", GLFW_KEY_LEFT, "Pan left (Editor)");
	bindControl("editorRight", GLFW_KEY_RIGHT, "Pan right (Editor)");
	bindControl("editorUp", GLFW_KEY_UP, "Pan up (Editor)");
	bindControl("editorDown", GLFW_KEY_DOWN, "Pan down (Editor)");
	bindControl("editorAntiClockwise", GLFW_KEY_RIGHT_CONTROL, "Rotate anti-clockwise (Editor)");
	bindControl("editorClockwise", GLFW_KEY_RIGHT_SHIFT, "Rotate clockwise (Editor)");
	bindControl("editorMenu", GLFW_KEY_ENTER, "Open editor menu");
	bindControl("interact", GLFW_KEY_SPACE, "Interact with objects");
	//Save the keybindings (some newer default bindings may have been missing)
	bindings.saveToFile("keybindings.do");
}


// Saves the key bindings to file
void KeyConfig::saveBindings() {
	DataObject bindings = DataObject();

	for (auto const& key : keyBindings) {
		bindings.add(key.first, key.second);
	}
	bindings.saveToFile("keybindings.do");
}


// Returns if a bound key is down
bool KeyConfig::isDown(string key) {
	return glfwGetKey(win, keyBindings[key]) == GLFW_PRESS;
}


// Binds a key to a binding
void KeyConfig::bind(string binding, int key) {
	keyBindings[binding] = key;
}


// Returns a list of bindings
string* KeyConfig::getBindings(int *size) {
	*size = keyBindings.size();
	string* r = new string[*size];
	int i = 0;
	for (auto const& key : keyBindings) {
		r[i] = key.first;
		i++;
	}
	return r;
}


// Gets the user friendly name of a binding
string KeyConfig::getFriendlyName(string binding) {
	if (bindingNames.find(binding)!=bindingNames.end()) {
		return bindingNames[binding];
	}
	return binding;
}


// Gets the key bound to a binding
int KeyConfig::getKey(string binding) {
	return keyBindings[binding];
}


string KeyConfig::getReadable(int key) {
	//GLFW seems to be stupidly broken in regards to this
	const char* name = glfwGetKeyName(key, 0);
	if (name != NULL) {
		return string(name);
	}
	switch (key) {
	case GLFW_KEY_BACKSPACE:
		return "Backspace";
	case GLFW_KEY_CAPS_LOCK:
		return "Caps Lock";
	case GLFW_KEY_DELETE:
		return "Delete";
	case GLFW_KEY_DOWN:
		return "Down";
	case GLFW_KEY_END:
		return "End";
	case GLFW_KEY_ENTER:
		return "Enter";
	case GLFW_KEY_ESCAPE:
		return "Escape";
	case GLFW_KEY_F1:
		return "F1";
	case GLFW_KEY_F2:
		return "F2";
	case GLFW_KEY_F3:
		return "F3";
	case GLFW_KEY_F4:
		return "F4";
	case GLFW_KEY_F5:
		return "F5";
	case GLFW_KEY_F6:
		return "F6";
	case GLFW_KEY_F7:
		return "F7";
	case GLFW_KEY_F8:
		return "F8";
	case GLFW_KEY_F9:
		return "F9";
	case GLFW_KEY_F10:
		return "F10";
	case GLFW_KEY_F11:
		return "F11";
	case GLFW_KEY_F12:
		return "F12";
	case GLFW_KEY_HOME:
		return "Home";
	case GLFW_KEY_INSERT:
		return "Insert";
	case GLFW_KEY_KP_ADD:
		return "NUM ADD";
	case GLFW_KEY_LEFT:
		return "Left";
	case GLFW_KEY_LEFT_ALT:
		return "Alt";
	case GLFW_KEY_LEFT_CONTROL:
		return "Left Control";
	case GLFW_KEY_LEFT_SHIFT:
		return "Left Shift";
	case GLFW_KEY_PAGE_DOWN:
		return "Page Down";
	case GLFW_KEY_PAGE_UP:
		return "Page Up";
	case GLFW_KEY_RIGHT:
		return "Right";
	case GLFW_KEY_RIGHT_ALT:
		return "Right Alt";
	case GLFW_KEY_RIGHT_CONTROL:
		return "Right Control";
	case GLFW_KEY_RIGHT_SHIFT:
		return "Right Shift";
	case GLFW_KEY_SPACE:
		return "Space";
	case GLFW_KEY_TAB:
		return "Tab";
	case GLFW_KEY_UP:
		return "Up";
	default:
		return "<"+to_string(key)+">";
	}
}