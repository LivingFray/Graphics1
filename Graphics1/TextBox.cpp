#include "TextBox.h"



TextBox::TextBox() {
	text = "";
	x = 0;
	y = 0;
	width = 100;
	height = 10;
}


TextBox::~TextBox() {
}


void TextBox::setText(string text) {
	this->text = text;
}


string TextBox::getText() {
	return text;
}


void TextBox::setX(int x) {
	this->x = x;
}


int TextBox::getX() {
	return x;
}


void TextBox::setY(int y) {
	this->y = y;
}


int TextBox::getY() {
	return y;
}


void TextBox::setWidth(int width) {
	this->width = width;
}


int TextBox::getWidth() {
	return width;
}


void TextBox::setHeight(int height) {
	this->height = height;
}


int TextBox::getHeight() {
	return height;
}


void TextBox::textEvent(unsigned int key) {
	if (selected) {
		text = text + (char)key;
	}
}


void TextBox::draw() {
	glColor3ub(255, 255, 255);
	glBegin(GL_QUADS);
	glVertex2i(x - width / 2, y - height / 2);
	glVertex2i(x + width / 2, y - height / 2);
	glVertex2i(x + width / 2, y + height / 2);
	glVertex2i(x - width / 2, y + height / 2);
	glEnd();
	glColor3ub(0, 0, 0);
	string d = text;
	if (selected) {
		d = d + "|";
	}
	freetype::print(fontSmall, x - width / 2 + 10, y - fontSmall.h * 0.325, d.c_str());
}


// Called when the left mouse button is clicked
void TextBox::mouseDown(int x, int y) {
	float w = width*0.5f;
	float h = height*0.5f;
	selected = x >= this->x - w && x <= this->x + w && y >= this->y - h && y <= this->y + h;
}


void TextBox::keyDown(int key, int scan, int action, int mods) {
	if (selected && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
		if (key == GLFW_KEY_BACKSPACE && text.length()>0) {
			text.erase(text.end() - 1);
		} else if (key == GLFW_KEY_ENTER || key == GLFW_KEY_ESCAPE) {
			selected = false;
		}
	}
}
