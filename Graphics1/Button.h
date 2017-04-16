#pragma once
#include "Globals.h"
class Button {
public:
	Button();
	~Button();
	// Sets the x-position of the button
	void setX(int x);
	// Gets the x-position of the button
	int getX();
	// Sets the y-position of the button
	void setY(int y);
	// Gets the y-position of the button
	int getY();
	// Sets the width of the button
	void setWidth(int width);
	// Gets the width of the button
	int getWidth();
	// Sets the height of the button
	void setHeight(int height);
	// Gets the height of the button
	int getHeight();
	// Sets the label on the button
	void setLabel(string label);
	// Gets the label of the button
	string getLabel();
	// Draw the button
	virtual void draw();
	// Draw the button with exrapolation
	virtual void draw(double ex);
	// Called when a key event is fired
	virtual void keyEvent(GLFWwindow* window, int key, int scan, int action, int mods);
protected:
	int x;
	int y;
	int width;
	int height;
	string label;
	GLuint img;
	GLuint imgSelected;
	void(*callback)(BaseState* s);
	freetype::font_data* font;
public:
	// Sets the function called when the button is pressed
	void setCallback(void(*callback)(BaseState* s));
	// Called when the left mouse button is clicked
	virtual bool mouseDown(int x, int y);
};

