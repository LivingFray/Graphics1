#include "LevelEditor.h"
#include "MainMenu.h"
#include "GradButton.h"
#define EDITOR_MOVE_SPEED 50
#define EDITOR_ROTATE_SPEED 30
#define MOVE_SIZE 50
#define SELECT_RADIUS 5
#define POS_SNAP 5
#define ANG_SNAP 5
#define ROTATE_SEGMENTS 36

LevelEditor::LevelEditor() {
	//Initialise variables
	inItemMenu = false;
	camPos = Vec2D(0.0, 0.0);
	camAngle = 0.0;
	current = 0;
	selected = NULL;
	panning = false;
	panFrom = Vec2D(0, 0);
	moving = false;
	moveDir = Vec2D(0, 0);
	resizing = 0;
	scaleDir = Vec2D(0, 0);
	//Create call list
	rotateCall = glGenLists(1);
	glNewList(rotateCall, GL_COMPILE);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < ROTATE_SEGMENTS; i++) {
		double x = MOVE_SIZE * cos(DEG_TO_RAD * i * (360 / ROTATE_SEGMENTS));
		double y = MOVE_SIZE * sin(DEG_TO_RAD * i * (360 / ROTATE_SEGMENTS));
		glVertex2d(x, y);
	}
	glEnd();
	glEndList();
	//Load button images
	barButtons[0] = ImageLoader::getImage("Resources\\selectButton.png");
	barButtons[1] = ImageLoader::getImage("Resources\\moveButton.png");
	barButtons[2] = ImageLoader::getImage("Resources\\resizeButton.png");
	barButtons[3] = ImageLoader::getImage("Resources\\rotateButton.png");
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
	glDeleteLists(rotateCall, 1);
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
	if (selected) {
		//Draw point showing centre of object
		Vec2D pos = selected->getPos();
		glColor3ub(255, 127, 0);
		glPointSize(8);
		glLineWidth(4);
		glBegin(GL_POINTS);
		glVertex2d(pos.getX(), pos.getY());
		glEnd();
		switch (current) {
		case 1: //Move
			if (!selected->canMove()) {
				break;
			}
			glBegin(GL_LINES);
			glVertex2d(pos.getX() + MOVE_SIZE, pos.getY());
			glVertex2d(pos.getX() - MOVE_SIZE, pos.getY());
			glVertex2d(pos.getX(), pos.getY() - MOVE_SIZE);
			glVertex2d(pos.getX(), pos.getY() + MOVE_SIZE);
			glEnd();
			glBegin(GL_POINTS);
			glVertex2d(pos.getX() + MOVE_SIZE, pos.getY());
			glVertex2d(pos.getX() - MOVE_SIZE, pos.getY());
			glVertex2d(pos.getX(), pos.getY() - MOVE_SIZE);
			glVertex2d(pos.getX(), pos.getY() + MOVE_SIZE);
			glEnd();
			break;
		case 2: //Resize
		{
			if (!selected->canResize()) {
				break;
			}
			double w = selected->getWidth() * 0.5;
			double h = selected->getHeight() * 0.5;
			glColor3ub(0, 127, 255);
			glPushMatrix();
			glTranslated(pos.getX(), pos.getY(), 0.0);
			glRotated(selected->getAngle(), 0.0, 0.0, 1.0);
			glTranslated(-pos.getX(), -pos.getY(), 0.0);
			glBegin(GL_POINTS);
			glVertex2d(pos.getX() + w, pos.getY());
			glVertex2d(pos.getX() - w, pos.getY());
			glVertex2d(pos.getX(), pos.getY() - h);
			glVertex2d(pos.getX(), pos.getY() + h);
			glEnd();
			glPopMatrix();
			break;
		}
		case 3: //Rotate
			if (!selected->canRotate()) {
				break;
			}
			glPushMatrix();
			glTranslated(pos.getX(), pos.getY(), 0.0);
			glColor3ub(0, 127, 0);
			glLineWidth(SELECT_RADIUS);
			glCallList(rotateCall);
			glPopMatrix();
			break;
		}
	}
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
		int bSize = (int)(sHeight * 0.05);
		//Draw editor bar
		glBegin(GL_QUADS);
		glColor4ub(33, 179, 242, 150);
		glVertex2i(sWidth, sHeight);
		glVertex2i(0, sHeight);
		glColor4ub(33, 179, 242, 10);
		glVertex2i(0, sHeight - bSize);
		glVertex2i(sWidth, sHeight - bSize);
		glEnd();
		glPushMatrix();
		//Buttons for add item, move item, rotate item, scale item
		for (int i = 0; i < EDITOR_BAR_BUTTONS; i++) {
			if (current == i) {
				//Draw button selected image
				glColor4ub(255, 255, 255, 50);
			}
			else {
				//Draw button image
				glColor4ub(255, 255, 255, 00);
			}
			glBegin(GL_QUADS);
			glVertex2i(0, sHeight);
			glVertex2i(0, sHeight - bSize);
			glVertex2i(bSize, sHeight - bSize);
			glVertex2i(bSize, sHeight);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, barButtons[i]);
			glEnable(GL_TEXTURE_2D);
			//Draw image
			glColor3ub(255, 255, 255);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0);
			glVertex2i(0, sHeight);
			glTexCoord2d(0.0, 0.0);
			glVertex2i(0, sHeight - bSize);
			glTexCoord2d(1.0, 0.0);
			glVertex2i(bSize, sHeight - bSize);
			glTexCoord2d(1.0, 1.0);
			glVertex2i(bSize, sHeight);
			glEnd();
			glTranslated(bSize, 0.0, 0.0);
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();
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
	y = sHeight - y;
	//Drag editor
	if (!inItemMenu && button == GLFW_MOUSE_BUTTON_RIGHT) {
		panning = action == GLFW_PRESS;
		if (panning) {
			panFrom = Vec2D(x, y);
			glfwSetCursor(window, cursorPan);
		}
		else {
			glfwSetCursor(window, cursorNormal);
		}
	}
	//Handle Button press stuff below
	if (button != GLFW_MOUSE_BUTTON_LEFT) {
		return;
	}
	//Handle clicks in menu
	if (inItemMenu) {
		if (action == GLFW_RELEASE) {
			for (GradButton* b : buttons) {
				b->mouseDown((int)x, (int)y);
			}
		}
	}
	else {
		//If in editor bar
		if (y >= sHeight * 0.95) {
			int index = (int)x / ((int)(sHeight * 0.05));
			if (action == GLFW_RELEASE && index < EDITOR_BAR_BUTTONS) {
				current = index;
			}
		}
		else {
			//Convert coords to world coords
			Vec2D world = getWorldCoordinates(Vec2D(x, y));
			Vec2D pos;
			if (selected) {
				pos = selected->getPos();
			}
			switch (current) {
			case 0: //Select
				selected = NULL;
				//TODO: Gravfields first
				for (Platform* p : platforms) {
					if (p->isInBoundingBox(world.getX(), world.getY())) {
						selected = p;
						break;
					}
				}
				//TODO: Entities
				break;
			case 1: //Move
			{
				//If the current object cannot be moved or doesn't exist
				if (!selected || !selected->canMove()) {
					break;
				}
				//Stop moving the object
				if (action == GLFW_RELEASE) {
					moving = false;
					break;
				}
				//For each direction check if mouse is grabbing
				//Possible vertical match
				if (world.getX() >= pos.getX() - SELECT_RADIUS && world.getX() <= pos.getX() + SELECT_RADIUS) {
					if (world.getY() >= pos.getY() + MOVE_SIZE - SELECT_RADIUS && world.getY() <= pos.getY() + MOVE_SIZE + SELECT_RADIUS) {
						moveDir = Vec2D(0, 1);
						moving = true;
					}
					else if (world.getY() >= pos.getY() - MOVE_SIZE - SELECT_RADIUS && world.getY() <= pos.getY() - MOVE_SIZE + SELECT_RADIUS) {
						moveDir = Vec2D(0, -1);
						moving = true;
					}
				}
				//Possible horizontal match
				else if (world.getY() >= pos.getY() - SELECT_RADIUS && world.getY() <= pos.getY() + SELECT_RADIUS) {
					if (world.getX() >= pos.getX() + MOVE_SIZE - SELECT_RADIUS && world.getX() <= pos.getX() + MOVE_SIZE + SELECT_RADIUS) {
						moveDir = Vec2D(1, 0);
						moving = true;
					}
					else if (world.getX() >= pos.getX() - MOVE_SIZE - SELECT_RADIUS && world.getX() <= pos.getX() - MOVE_SIZE + SELECT_RADIUS) {
						moveDir = Vec2D(-1, 0);
						moving = true;
					}
				}
				break;
			}
			case 2: //Resize
			{
				//If the current object cannot be resized or doesn't exist
				if (!selected || !selected->canMove()) {
					break;
				}
				//Stop resizing the object
				if (action == GLFW_RELEASE) {
					resizing = 0;
					break;
				}
				//Determine vectors handles lie upon
				Vec2D width = Vec2D(cos(selected->getAngle() * DEG_TO_RAD), sin(selected->getAngle() * DEG_TO_RAD));
				Vec2D height = Vec2D(-width.getY(), width.getX());
				width.multiplyBy(selected->getWidth() * 0.5);
				height.multiplyBy(selected->getHeight() * 0.5);
				//Check each handle to see if it is being clicked on
				Vec2D handle = pos.add(width);
				if (world.getX() >= handle.getX() - SELECT_RADIUS && world.getX() <= handle.getX() + SELECT_RADIUS
					&& world.getY() >= handle.getY() - SELECT_RADIUS && world.getY() <= handle.getY() + SELECT_RADIUS) {
					scaleDir = width;
					resizing = 1;
					break;
				}
				handle = pos.subtract(width);
				if (world.getX() >= handle.getX() - SELECT_RADIUS && world.getX() <= handle.getX() + SELECT_RADIUS
					&& world.getY() >= handle.getY() - SELECT_RADIUS && world.getY() <= handle.getY() + SELECT_RADIUS) {
					scaleDir = width.multiply(-1);
					resizing = 2;
					break;
				}
				handle = pos.add(height);
				if (world.getX() >= handle.getX() - SELECT_RADIUS && world.getX() <= handle.getX() + SELECT_RADIUS
					&& world.getY() >= handle.getY() - SELECT_RADIUS && world.getY() <= handle.getY() + SELECT_RADIUS) {
					scaleDir = height;
					resizing = 3;
					break;
				}
				handle = pos.subtract(height);
				if (world.getX() >= handle.getX() - SELECT_RADIUS && world.getX() <= handle.getX() + SELECT_RADIUS
					&& world.getY() >= handle.getY() - SELECT_RADIUS && world.getY() <= handle.getY() + SELECT_RADIUS) {
					scaleDir = height.multiply(-1);
					resizing = 4;
					break;
				}
				break;
			}
			case 3: //Rotate
			{
				//If the current object can't be rotated or doesn't exist
				if (!selected || !selected->canRotate()) {
					break;
				}
				//Stop rotating the object
				if (action == GLFW_RELEASE) {
					rotating = false;
					break;
				}
				double d = world.subtract(pos).magnitudeSquare();
				double min = MOVE_SIZE - SELECT_RADIUS;
				double max = MOVE_SIZE + SELECT_RADIUS;
				if (d >= min * min && d <= max * max) {
					rotating = true;
					Vec2D dif = world.subtract(pos);
					rotateFrom = atan2(dif.getY(), dif.getX());
				}
				break;
			}
			}
		}
	}
}


// Called when a mouse move event is fired
void LevelEditor::mouseMoveEvent(GLFWwindow* window, double x, double y) {
	//The camera is being moved
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
	//A selectable is being translated
	if (moving) {
		//Move the object so the correct point is in line with the mouse
		Vec2D handle = moveDir.multiply(MOVE_SIZE).add(selected->getPos());
		Vec2D dif = getWorldCoordinates(Vec2D(x, sHeight - y)).subtract(handle);
		//Move up and down
		if (moveDir.getX() == 0.0) {
			selected->onMove(0, dif.getY());
		}
		else {
			selected->onMove(dif.getX(), 0);
		}
	}
	//A selectable is being resized
	if (resizing) {
		//Resize the object so the correct point is in line with the mouse
		Vec2D handle = scaleDir.add(selected->getPos());
		Vec2D dif = getWorldCoordinates(Vec2D(x, sHeight - y)).subtract(handle);
		double dis = scaleDir.dot(dif) / scaleDir.magnitude();
		bool resized;
		if (resizing <= 2) {
			resized = selected->onResize(dis, 0);
		}
		else {
			resized = selected->onResize(0, dis);
		}
		//Only move centre if could resize object
		if (resized) {
			Vec2D translate = scaleDir.unit().multiply(dis * 0.5);
			selected->onMove(translate.getX(), translate.getY());
			//Resize scaling to match new position
			scaleDir.addTo(scaleDir.unit().multiply(dis * 0.5));
		}
	}
	//A selectable is being rotated
	if (rotating) {
		Vec2D dif = getWorldCoordinates(Vec2D(x, sHeight - y)).subtract(selected->getPos());
		double angle = atan2(dif.getY(), dif.getX());
		selected->onRotate(RAD_TO_DEG * (angle - rotateFrom));
		rotateFrom = angle;
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
	bool move = false;
	bool rot = false;
	if (KeyConfig::isDown("editorLeft")) {
		dX -= time * EDITOR_MOVE_SPEED;
		move = true;
	}
	if (KeyConfig::isDown("editorRight")) {
		dX += time * EDITOR_MOVE_SPEED;
		move = true;
	}
	if (KeyConfig::isDown("editorUp")) {
		dY += time * EDITOR_MOVE_SPEED;
		move = true;
	}
	if (KeyConfig::isDown("editorDown")) {
		dY -= time * EDITOR_MOVE_SPEED;
		move = true;
	}
	if (KeyConfig::isDown("editorAntiClockwise")) {
		camAngle += time * EDITOR_ROTATE_SPEED;
		rot = true;
	}
	if (KeyConfig::isDown("editorClockwise")) {
		camAngle -= time * EDITOR_ROTATE_SPEED;
		rot = true;
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
	//Snapping
	if (!move && !panning) {
		double sX, sY;
		sX = floor(camPos.getX() / POS_SNAP) * POS_SNAP;
		camPos.setX(sX);
		sY = floor(camPos.getY() / POS_SNAP) * POS_SNAP;
		camPos.setY(sY);
	}
	if (!rot) {
		camAngle = floor(camAngle / ANG_SNAP) * ANG_SNAP;
	}
}
