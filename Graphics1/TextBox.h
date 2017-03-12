#pragma once
#include "Globals.h"
class TextBox {
public:
	TextBox();
	~TextBox();
	void setText(string text);
	string getText();
	void setX(int x);
	int getX();
	void setY(int y);
	int getY();
	void setWidth(int width);
	int getWidth();
	void setHeight(int height);
	int getHeight();
	void textEvent(unsigned int key);
	void draw();
	void mouseDown(int x, int y);
	void keyDown(int key, int scan, int action, int mods);
protected:
	string text;
	int x;
	int y;
	int width;
	int height;
	bool selected;
};

