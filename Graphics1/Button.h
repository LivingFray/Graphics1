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
	void setLabel(string label);
	// Gets the label of the button
	string getLabel();
	// Draw the button
	virtual void draw();
	// Draw the button with exrapolation
	virtual void draw(double ex);
protected:
	int x;
	int y;
	int width;
	int height;
	string label;
	GLuint img;
	void(*callback)(BaseState* s);
public:
	// Sets the function called when the button is pressed
	void setCallback(void(*callback)(BaseState* s));
	// Called when the left mouse button is clicked
	virtual void mouseDown(int x, int y);
};

