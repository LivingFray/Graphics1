#pragma once
#include "Globals.h"
class Button {
public:
	Button();
	~Button();
	// Sets the x-position of the button
	void Button::setX(int x);
	// Gets the x-position of the button
	int Button::getX();
	// Sets the y-position of the button
	void Button::setY(int y);
	// Gets the y-position of the button
	int Button::getY();
	// Sets the width of the button
	void Button::setWidth(int width);
	// Gets the width of the button
	int Button::getWidth();
	// Sets the height of the button
	void Button::setHeight(int height);
	// Gets the height of the button
	int Button::getHeight();
	// Sets the label on the button
	void setLabel(std::string label);
	// Gets the label of the button
	std::string getLabel();
	// Draw the button
	virtual void draw();
	virtual void draw(double ex);
protected:
	int x;
	int y;
	int width;
	int height;
	std::string label;
	GLuint img;
	void(*callback)();
public:
	// Sets the function called when the button is pressed
	void setCallback(void(*callback)());
	// Called when the left mouse button is clicked
	virtual void mouseDown(int x, int y);
};

