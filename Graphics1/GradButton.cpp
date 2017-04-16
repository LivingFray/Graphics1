#include "GradButton.h"



GradButton::GradButton() {
}


GradButton::~GradButton() {
}


// Draws the button ex seconds after last update
void GradButton::draw(double ex) {
	//TODO: Hover animation
	//Used to ensure clicking works
	x = sWidth - width / 2;
	glBegin(GL_POLYGON);
	glColor4ub(33, 179, 242, 150);
	glVertex2i(sWidth, y - height / 2);
	glVertex2i(sWidth, y + height / 2);
	glColor4ub(33, 179, 242, 10);
	glVertex2i(sWidth - width + height, y + height / 2);
	glVertex2i(sWidth - width, y - height / 2);
	glEnd();
	glColor3ub(255, 255, 255);
	freetype::print(fontLarge, (float)(sWidth - width + height), (float)y - fontLarge.h * 0.375f, label.c_str());
}
