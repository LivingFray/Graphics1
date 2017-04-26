#pragma once
#include "Button.h"
class GradButton:
	public Button {
public:
	GradButton();
	virtual ~GradButton();
	// Draws the button ex seconds after last update
	void draw(double ex);
};

