#include "SettingsMenu.h"
#include "BindingButton.h"
#include "GradButton.h"
#include "MainMenu.h"


SettingsMenu::SettingsMenu() {
	int s;
	string* bindings = KeyConfig::getBindings(&s);
	for (int i = 0; i < s; i++) {
		BindingButton* b = new BindingButton();
		b->setBinding(bindings[i]);
		b->setFriendlyName(KeyConfig::getFriendlyName(bindings[i]));
		buttons.push_back(b);
	}
	auto callExit = [](BaseState* s) {
		KeyConfig::saveBindings();
		newState = new MainMenu();
	};
	GradButton* b = new GradButton();
	b->setLabel("Save and Exit");
	b->setCallback(callExit);
	buttons.push_back(b);
}


SettingsMenu::~SettingsMenu() {
}


// Called when the window is resized
void SettingsMenu::resizeEvent(GLFWwindow* window, int width, int height) {
	int i = 0;
	int n = buttons.size();
	for (Button* b : buttons) {
		int h = (int)fontSmall.h * 2;
		b->setWidth((int)(sWidth * 0.9));
		b->setX(sWidth / 2);
		b->setY(sHeight - h * (i + 2));
		b->setHeight(h);
		i++;
		if (i == n) {
			b->setWidth(sWidth / 2);
			b->setY((int)fontLarge.h * 2);
			b->setHeight((int)(fontLarge.h * 2));
		}
	}
}


// Called when a key event is fired
void SettingsMenu::keyEvent(GLFWwindow* window, int key, int scan, int action, int mods) {
	for (Button* b : buttons) {
		b->keyEvent(window, key, scan, action, mods);
	}
}
