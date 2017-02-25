#include "GradButton.h"



GradButton::GradButton() {
}


GradButton::~GradButton() {
}


// Draws the button ex seconds after last update
void GradButton::draw(double ex) {

	//TODO: Hover animation
	glBegin(GL_POLYGON);
	glColor4ub(33, 179, 242, 150);
	glVertex2i(sWidth, y - height / 2);
	glVertex2i(sWidth, y + height / 2);
	glColor4ub(33, 179, 242, 10);
	glVertex2i(sWidth - width + height, y + height / 2);
	glVertex2i(sWidth - width, y - height / 2);
	glEnd();
	glColor3ub(255, 255, 255);
	freetype::print(font, (float)(sWidth - width + height), (float)y - font.h * 0.375f, label.c_str());
}

// Called when the left mouse button is clicked
void GradButton::mouseDown(int x, int y) {
	float h = height*0.5f;
	if (x >= sWidth - width && y >= this->y - h && y <= this->y + h) {
		(*callback)(state);
	}
}
