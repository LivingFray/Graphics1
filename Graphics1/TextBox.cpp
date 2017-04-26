#include "TextBox.h"
#include <stdlib.h>


TextBox::TextBox() {
	text = "";
	oldText = "";
	x = 0;
	y = 0;
	width = 100;
	height = 10;
	selected = false;
	numeric = false;
}


TextBox::~TextBox() {
}


void TextBox::setText(string text) {
	//Remove trailing zeros
	if(numeric && text.find_first_of('.')!=string::npos){
		text.erase(text.find_last_not_of('0') + 1, string::npos);
		if (text.size()>0 && text.at(text.size() - 1) == '.') {
			text.erase(text.size() - 1);
		}
	}
	this->text = text;
}


string TextBox::getText() {
	if (numeric && text == "") {
		return "0";
	}
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
	string d = " " + text;
	if (selected) {
		d = d + "|";
	}
	freetype::print(fontSmall, x - width / 2.0f, y - fontSmall.h * 0.325f, d.c_str());
}


// Called when the left mouse button is clicked
void TextBox::mouseDown(int x, int y) {
	float w = width*0.5f;
	float h = height*0.5f;
	selected = x >= this->x - w && x <= this->x + w && y >= this->y - h && y <= this->y + h;
	if (!selected) {
		validate();
	}
}


void TextBox::keyDown(int key, int scan, int action, int mods) {
	if (selected && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
		if (key == GLFW_KEY_BACKSPACE && text.length()>0) {
			text.erase(text.end() - 1);
		} else if (key == GLFW_KEY_ENTER || key == GLFW_KEY_ESCAPE) {
			selected = false;
			validate();
		}
	}
}


// Validates the input and resets it if it fails
void TextBox::validate() {
	if (numeric) {
		char* end;
		strtod(text.c_str(), &end);
		//If it cannot be converted
		if (*end) {
			text = oldText;
		} else {
			setText(text);
		}
	}
	oldText = text;
}

// Sets whether the text box only accepts numerical values
void TextBox::setNumeric(bool n) {
	numeric = n;
	if (n) {
		validate();
	}
}


// Gets whether the textbox only accepts numerical values
bool TextBox::getNumeric() {
	return numeric;
}


// Gets whether the textbox is selected
bool TextBox::getSelected() {
	return selected;
}
