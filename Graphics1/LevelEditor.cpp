#include "LevelEditor.h"
#include "MainMenu.h"
#include "GradButton.h"
#define EDITOR_MOVE_SPEED 50
#define EDITOR_ROTATE_SPEED 90

LevelEditor::LevelEditor() {
	//Initialise variables
	inItemMenu = false;
	camPos = Vec2D(0.0, 0.0);
	camAngle = 0.0;
	//Create elements of spawn menu
	MenuItem item;
	//Reset item
	item = MenuItem();
	item.name = "TEST Platform";
	item.create = [](BaseState* s) {
		LevelEditor* l = (LevelEditor*)s;
		Platform* pl = new Platform();
		pl->setPos(l->getCameraPos());
		pl->setWidth(50);
		pl->setHeight(50);
		pl->setAngle(l->getCameraAngleAt(0));
		l->addPlatform(pl);
		l->setInItemMenu(false);
	};
	menuItems.push_back(item);
	//Create buttons for menu
	for (MenuItem i : menuItems) {
		GradButton* b = new GradButton();
		int h = sHeight / 12;
		b->setY(0);
		b->setHeight((int)(h * 0.9));
		b->setLabel(i.name);
		b->setCallback(i.create);
		buttons.push_back(b);
	}
	//Test
	loadLevel("tmp");
}


LevelEditor::~LevelEditor() {
}


// Updates the editor
void LevelEditor::update() {
	//Move camera (tick only)
	updateCamera(TICKRATE);
}

// Draws the level
void LevelEditor::draw(double ex) {
	//Move camera (extrapolation only)
	updateCamera(ex);
	//Draw the level thus far 
	LevelRenderer::draw(ex);
	//Draw movement arrows/ resize arrows

	//Reset viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, sWidth, 0.0, sHeight, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//Draw menu if open
	if (inItemMenu) {
		glColor4ub(0, 0, 0, 127);
		glBegin(GL_QUADS);
		glVertex2i(0, 0);
		glVertex2i(0, sHeight);
		glVertex2i(sWidth, sHeight);
		glVertex2i(sWidth, 0);
		glEnd();
		glColor3ub(255, 255, 255);
		freetype::print(fontLarge, 10, sHeight - fontLarge.h * 1.325f, "Select An Item To Add...");
		//Draw menu buttons
		//TODO: Scrolling, smaller font
		int n = buttons.size();
		int i = 0;
		for (GradButton* b : buttons) {
			int h = sHeight / 12;
			b->setY((int)(sHeight - fontLarge.h * 1.325f - h * (i + 1)));
			b->setWidth((int)(sWidth * 0.5 + h * i));
			b->setHeight((int)(h * 0.9));
			b->draw(ex);
			i++;
		}
	}
	else {
		//Draw editor bar
		glBegin(GL_QUADS);
		glColor4ub(33, 179, 242, 150);
		glVertex2i(sWidth, sHeight);
		glVertex2i(0, sHeight);
		glColor4ub(33, 179, 242, 10);
		glVertex2i(0, (int)(sHeight * 0.95));
		glVertex2i(sWidth, (int)(sHeight * 0.95));
		glEnd();
		//TODO: buttons for add item, move item, rotate item, scale item

		//Draw coordinates
		glColor3ub(255, 255, 255);
		freetype::print(fontSmall, 10, fontSmall.h * 2.625f, "Position: (%.2f, %.2f)\nAngle: %.2f", camPos.getX(), camPos.getY(), camAngle);
	}
	//Draw edit menu (fine tune properties)
}

void LevelEditor::keyEvent(GLFWwindow * window, int key, int scan, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		if (inItemMenu) {
			//TODO: Any necessary things
			inItemMenu = false;
		}
		else {
			//TODO: Save
			newState = new MainMenu();
		}
	}
	if (key == KeyConfig::keyBindings["editorMenu"] && action == GLFW_RELEASE) {
		inItemMenu = !inItemMenu;
		//Fix cursor
		if (panning) {
			glfwSetCursor(window, cursorNormal);
			panning = false;
		}
		//TODO: handle closing menu?
	}
}

// Called when a mouse event is fired
void LevelEditor::mouseEvent(GLFWwindow* window, int button, int action, int mods) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	//Drag editor
	if (!inItemMenu && button == GLFW_MOUSE_BUTTON_RIGHT) {
		panning = action == GLFW_PRESS;
		if (panning) {
			panFrom = Vec2D(x, sHeight - y);
			glfwSetCursor(window, cursorPan);
		}
		else {
			glfwSetCursor(window, cursorNormal);
		}
	}
	//Handle Button press stuff below
	y = sHeight - y;
	if (action != GLFW_RELEASE) {
		return;
	}
	//Handle clicks in menu
	if (inItemMenu) {
		for (GradButton* b : buttons) {
			b->mouseDown((int)x, (int)y);
		}
	}
}


// Called when a mouse move event is fired
void LevelEditor::mouseMoveEvent(GLFWwindow* window, double x, double y) {
	if (panning) {
		double scale = WORLD_SIZE / (double)(sWidth < sHeight ? sWidth : sHeight);
		//TODO: rotate camera
		Vec2D dif = Vec2D(x, sHeight - y).subtract(panFrom);
		dif.multiplyBy(scale);
		double h = dif.magnitude();
		double ang = atan2(dif.getY(), dif.getX());
		if (ang < 0) {
			ang = TAU + ang;
		}
		ang += camAngle * DEG_TO_RAD;
		dif.setX(h * cos(ang));
		dif.setY(h * sin(ang));
		camPos.subtractFrom(dif);
		panFrom = Vec2D(x, sHeight - y);
	}
}

// Sets whether the item menu is visible
void LevelEditor::setInItemMenu(bool inMenu) {
	inItemMenu = inMenu;
}


// Gets the current position of the camera
Vec2D LevelEditor::getCameraPos() {
	return camPos;
}


// Gets the camera position ex seconds after last update
Vec2D LevelEditor::getCameraAt(double ex) {
	return camPos;
}


// Gets the angle of the camera ex seconds after last update
double LevelEditor::getCameraAngleAt(double ex) {
	return camAngle;
}



// Updates the camera position and rotation
void LevelEditor::updateCamera(double time) {
	double dX = 0.0, dY = 0.0;
	if (KeyConfig::isDown("editorLeft")) {
		dX -= time * EDITOR_MOVE_SPEED;
	}
	if (KeyConfig::isDown("editorRight")) {
		dX += time * EDITOR_MOVE_SPEED;
	}
	if (KeyConfig::isDown("editorUp")) {
		dY += time * EDITOR_MOVE_SPEED;
	}
	if (KeyConfig::isDown("editorDown")) {
		dY -= time * EDITOR_MOVE_SPEED;
	}
	if (KeyConfig::isDown("editorAntiClockwise")) {
		camAngle += time * EDITOR_ROTATE_SPEED;
	}
	if (KeyConfig::isDown("editorClockwise")) {
		camAngle -= time * EDITOR_ROTATE_SPEED;
	}
	double h = sqrt(dX * dX + dY * dY);
	double ang = atan2(dY, dX);
	if (ang < 0) {
		ang = TAU + ang;
	}
	ang += camAngle * DEG_TO_RAD;
	dX = h * cos(ang);
	dY = h * sin(ang);
	//Keep angle between 0 and 360
	if (camAngle < 0) {
		camAngle += 360;
	}
	if (camAngle >= 360) {
		camAngle -= 360;
	}
	camPos.addTo(Vec2D(dX, dY));
}