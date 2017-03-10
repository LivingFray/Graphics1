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
protected:
	string text;
	int x;
	int y;
	int width;
	int height;
};

