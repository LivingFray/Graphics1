#pragma once
#include "BaseState.h"
#include "Button.h"

class Menu: public BaseState {
public:
	Menu();
	~Menu();
	// Draws the main menu ex seconds after last update
	void draw(double ex);
	// Updates the menu
	void update();
protected:
	Button test;
	GLuint planet;
	GLuint stars;
};

