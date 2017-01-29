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

keyMap keyconfig::keyBindings;

// Loads default keybindings then sets custom bindings where available
void keyconfig::loadBindings() {
	DataObject* bindings = new DataObject();
	bindings->loadFromFile("keybindings.do");
	DATATYPE datatype;
	//By the power of macros, check if each key is bound and rebind if possible
	bindControl("moveLeft",GLFW_KEY_A);
	bindControl("moveRight", GLFW_KEY_D);
	bindControl("jump", GLFW_KEY_W);
	//Save the keybindings (some newer default bindings may have been missing)
	bindings->saveToFile("keybindings.do");
	delete bindings;
}

void keyconfig::saveBindings() {
	DataObject* bindings = new DataObject();

	for (auto const& key : keyBindings) {
		int v = key.second;
		bindings->add(key.first, INT, &v);
	}
	bindings->saveToFile("keybindings.do");
	delete bindings;
}