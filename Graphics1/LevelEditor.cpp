#include "LevelEditor.h"
#include "MainMenu.h"
#include "GradButton.h"
#include "SpawnPoint.h"
#include "Goal.h"
#include "PointGiver.h"
#include "BombEntity.h"
#include "ShieldGiver.h"
#define EDITOR_MOVE_SPEED 1.00
#define EDITOR_ROTATE_SPEED 30
#define MOVE_SIZE 0.5
#define SELECT_RADIUS 0.05
#define POS_SNAP 0.05
#define ANG_SNAP 0.05
#define ROTATE_SEGMENTS 36

LevelEditor::LevelEditor() {
	//Initialise variables
	currentMenu = Menu::NONE;
	camPos = Vec2D(0.0, 0.0);
	camAngle = 0.0;
	current = 0;
	selected = NULL;
	panning = false;
	panFrom = Vec2D(0, 0);
	moving = false;
	moveDir = Vec2D(0, 0);
	resizing = 0;
	levelName = "Untitled";
	defaultGravity = 0;
	scaleDir = Vec2D(0, 0);
	saveButton = Button();
	saveButton.setLabel("Save level");
	auto saveCall = [](BaseState* s) {
		LevelEditor* e = (LevelEditor*)s;
		e->saveLevel(e->getFileLocation());
	};
	saveButton.setCallback(saveCall);
	loadButton = Button();
	loadButton.setLabel("Load level");
	auto loadCall = [](BaseState* s) {
		LevelEditor* e = (LevelEditor*)s;
		e->loadLevel(e->getFileLocation());
	};
	loadButton.setCallback(loadCall);
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
	barButtons[0] = ImageLoader::getImage("Resources\\buttons\\select.png");
	barButtons[1] = ImageLoader::getImage("Resources\\buttons\\move.png");
	barButtons[2] = ImageLoader::getImage("Resources\\buttons\\resize.png");
	barButtons[3] = ImageLoader::getImage("Resources\\buttons\\rotate.png");
	barButtons[4] = ImageLoader::getImage("Resources\\buttons\\options.png");
	barButtons[5] = ImageLoader::getImage("Resources\\buttons\\delete.png");
	//Create elements of spawn menu
	MenuItem item;
	//Reset item
	//TODO: Better adding method, macro?
	item = MenuItem();
	item.name = "TEST Platform";
	item.create = [](BaseState* s) {
		LevelEditor* l = (LevelEditor*)s;
		Platform* pl = new Platform();
		pl->setPos(l->getCameraPos());
		pl->setWidth(1.00);
		pl->setHeight(1.00);
		pl->setAngle(l->getCameraAngleAt(0));
		l->addPlatform(pl);
		l->setMenu(0);
	};
	menuItems.push_back(item);
	item = MenuItem();
	item.name = "TEST GravField";
	item.create = [](BaseState* s) {
		LevelEditor* l = (LevelEditor*)s;
		GravityField* f = new GravityField();
		f->setPos(l->getCameraPos());
		f->setWidth(10.00);
		f->setHeight(10.00);
		f->setAngle(l->getCameraAngleAt(0));
		f->setStrength(10.00);
		l->addGravityField(f);
		l->setMenu(0);
	};
	menuItems.push_back(item);
	item = MenuItem();
	item.name = "TEST Points";
	item.create = [](BaseState* s) {
		LevelEditor* l = (LevelEditor*)s;
		PointGiver* p = new PointGiver();
		p->setPos(l->getCameraPos());
		p->setAngle(l->getCameraAngleAt(0));
		l->addEntity(p);
		l->setMenu(0);
	};
	menuItems.push_back(item);
	item = MenuItem();
	item.name = "TEST Bomb";
	item.create = [](BaseState* s) {
		LevelEditor* l = (LevelEditor*)s;
		BombEntity* p = new BombEntity();
		p->setPos(l->getCameraPos());
		p->setAngle(l->getCameraAngleAt(0));
		l->addEntity(p);
		l->setMenu(0);
	};
	menuItems.push_back(item);
	item = MenuItem();
	item.name = "TEST Shield";
	item.create = [](BaseState* s) {
		LevelEditor* l = (LevelEditor*)s;
		ShieldGiver* p = new ShieldGiver();
		p->setPos(l->getCameraPos());
		p->setAngle(l->getCameraAngleAt(0));
		l->addEntity(p);
		l->setMenu(0);
	};
	menuItems.push_back(item);
	//Create buttons for menu
	for (MenuItem i : menuItems) {
		GradButton* b = new GradButton();
		int h = (int)fontLarge.h * 2;
		b->setY(0);
		b->setHeight((int)(h * 0.9));
		b->setLabel(i.name);
		b->setCallback(i.create);
		buttons.push_back(b);
	}
	fileBox = TextBox();
	fileBox.setText("");
	levelBox = TextBox();
	levelBox.setText("");
	nextBox = TextBox();
	nextBox.setText("");
	gravBox = TextBox();
	gravBox.setText("");
	gravBox.setNumeric(true);

	exitButton = GradButton();
	exitButton.setLabel("Exit to menu");
	auto exitCall = [](BaseState* s) {
		state = new MainMenu();
	};
	exitButton.setCallback(exitCall);
	returnButton = GradButton();
	returnButton.setLabel("Return to level");
	auto returnCall = [](BaseState* s) {
		LevelEditor* l = (LevelEditor*)s;
		l->setMenu((int)Menu::NONE);
	};
	returnButton.setCallback(returnCall);
	//TODO: Start with choosing a level
	loadLevel("");
}


LevelEditor::~LevelEditor() {
	glDeleteLists(rotateCall, 1);
	//Clear up any lingering options menus
	if (optMenu) {
		delete optMenu;
	}
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
	//Draw movement arrows/resize arrows
	if (selected) {
		//Draw point showing centre of object
		Vec2D pos = selected->getPos();
		glColor3ub(255, 127, 0);
		glLineWidth(4);
		drawHandle(pos.getX(), pos.getY());
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
			drawHandle(pos.getX() + MOVE_SIZE, pos.getY());
			drawHandle(pos.getX() - MOVE_SIZE, pos.getY());
			drawHandle(pos.getX(), pos.getY() - MOVE_SIZE);
			drawHandle(pos.getX(), pos.getY() + MOVE_SIZE);
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
			drawHandle(pos.getX() + w, pos.getY());
			drawHandle(pos.getX() - w, pos.getY());
			drawHandle(pos.getX(), pos.getY() - h);
			drawHandle(pos.getX(), pos.getY() + h);
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
	if (currentMenu == Menu::ITEM) {
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
	} else if (currentMenu == Menu::SAVE) {
		glColor4ub(0, 0, 0, 127);
		glBegin(GL_QUADS);
		glVertex2i(0, 0);
		glVertex2i(0, sHeight);
		glVertex2i(sWidth, sHeight);
		glVertex2i(sWidth, 0);
		glEnd();
		glColor3ub(255, 255, 255);
		freetype::print(fontLarge, 10, sHeight - fontLarge.h * 1.325f, "Settings");
		int w = (int)(sWidth * 0.25);
		int h = (int)(sHeight * 0.1);
		int y = (int)(sHeight * 0.3);
		//Save button
		saveButton.setWidth(w);
		saveButton.setHeight(h);
		saveButton.setX((int)(sWidth * 0.5 - w * 0.75));
		saveButton.setY(y);
		saveButton.draw();
		//Load button
		loadButton.setWidth(w);
		loadButton.setHeight(h);
		loadButton.setX((int)(sWidth * 0.5 + w * 0.75));
		loadButton.setY(y);
		loadButton.draw();
		//Level name
		y = sHeight - (int)fontLarge.h * 4;
		drawTextBox("Level name", levelBox, y);
		//File location
		y -= (int)fontSmall.h * 4;
		drawTextBox("File location", fileBox, y);
		//Default gravity
		y -= (int)fontSmall.h * 4;
		drawTextBox("Default gravity", gravBox, y);
		y -= (int)fontSmall.h * 4;
		drawTextBox("Next level", nextBox, y);
		//Exit button
		exitButton.setY((int)(fontLarge.h * 2));
		exitButton.setHeight((int)(fontLarge.h * 1.9));
		exitButton.setWidth(w * 2);
		exitButton.draw(0);
		//Return button
		returnButton.setY((int)(fontLarge.h * 4));
		returnButton.setWidth((w - (int)fontLarge.h) * 2);
		returnButton.setHeight((int)(fontLarge.h * 1.9));
		returnButton.draw(0);
	} else if (currentMenu == Menu::OPTIONS) {
		//Draw options menu for object
		glColor4ub(0, 0, 0, 127);
		glBegin(GL_QUADS);
		glVertex2i(0, 0);
		glVertex2i(0, sHeight);
		glVertex2i(sWidth, sHeight);
		glVertex2i(sWidth, 0);
		glEnd();
		glColor3ub(255, 255, 255);
		freetype::print(fontLarge, 10, sHeight - fontLarge.h * 1.325f, "Object options");
		optMenu->draw();
		returnButton.setY((int)(fontLarge.h * 2));
		returnButton.setHeight((int)(fontLarge.h * 2));
		returnButton.setWidth(sWidth / 2);
		returnButton.draw(0);
	} else {
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
			} else {
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
}


void LevelEditor::keyEvent(GLFWwindow * window, int key, int scan, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		if (currentMenu != Menu::NONE) {
			currentMenu = Menu::NONE;
		} else {
			currentMenu = Menu::SAVE;
			selected = NULL;
			//Fix cursor
			if (panning) {
				glfwSetCursor(window, cursorNormal);
				panning = false;
			}
		}
	}
	if (currentMenu == Menu::SAVE) {
		fileBox.keyDown(key, scan, action, mods);
		levelBox.keyDown(key, scan, action, mods);
		nextBox.keyDown(key, scan, action, mods);
		gravBox.keyDown(key, scan, action, mods);
	} else if (currentMenu == Menu::OPTIONS) {
		optMenu->keyEvent(key, scan, action, mods);
		//Save changes
		if (key == GLFW_KEY_ENTER) {
			selected->setOptions(optMenu);
		}
	}
	if (key == KeyConfig::keyBindings["editorMenu"] && action == GLFW_RELEASE) {
		if (currentMenu == Menu::NONE) {
			currentMenu = Menu::ITEM;
			//Fix cursor
			if (panning) {
				glfwSetCursor(window, cursorNormal);
				panning = false;
			}
		} else if (currentMenu == Menu::ITEM) {
			currentMenu = Menu::NONE;
		}
	}
}


// Called when a mouse event is fired
void LevelEditor::mouseEvent(GLFWwindow* window, int button, int action, int mods) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	y = sHeight - y;
	//Drag editor
	if (currentMenu == Menu::NONE && button == GLFW_MOUSE_BUTTON_RIGHT) {
		panning = action == GLFW_PRESS;
		if (panning) {
			panFrom = Vec2D(x, y);
			glfwSetCursor(window, cursorPan);
		} else {
			glfwSetCursor(window, cursorNormal);
		}
	}
	//Handle Button press stuff below
	if (button != GLFW_MOUSE_BUTTON_LEFT) {
		return;
	}
	//Handle clicks in menu
	if (currentMenu == Menu::ITEM) {
		if (action == GLFW_RELEASE) {
			for (GradButton* b : buttons) {
				b->mouseDown((int)x, (int)y);
			}
		}
	} else if (currentMenu == Menu::SAVE) {
		if (action == GLFW_RELEASE) {
			loadButton.mouseDown((int)x, (int)y);
			saveButton.mouseDown((int)x, (int)y);
			exitButton.mouseDown((int)x, (int)y);
			returnButton.mouseDown((int)x, (int)y);
			fileBox.mouseDown((int)x, (int)y);
			levelBox.mouseDown((int)x, (int)y);
			nextBox.mouseDown((int)x, (int)y);
			gravBox.mouseDown((int)x, (int)y);
		}
	} else if (currentMenu == Menu::OPTIONS) {
		optMenu->mouseEvent(button, action, mods);
		if (action == GLFW_RELEASE) {
			returnButton.mouseDown((int)x, (int)y);
		}
	} else {
		//If in editor bar
		if (y >= sHeight * 0.95) {
			int index = (int)x / ((int)(sHeight * 0.05));
			if (action == GLFW_RELEASE && index < EDITOR_BAR_BUTTONS) {
				current = index;
				//Select options menu
				if (index == 4 && selected) {
					currentMenu = Menu::OPTIONS;
					selected->createOptions();
					optMenu = selected->getOptions();
				}
				return;
			}
		}
		if (action == GLFW_PRESS && y >= sHeight * 0.95) {
			return;
		}
		//Convert coords to world coords
		Vec2D world = getWorldCoordinates(Vec2D(x, y));
		Vec2D pos;
		if (selected) {
			pos = selected->getPos();
		}
		switch (current) {
		case 0: //Select
			if (action == GLFW_PRESS) {
				select(world);
			}
		case 1: //Move
		{
			//If the current object cannot be moved or doesn't exist
			if (!selected || !selected->canMove()) {
				if (action == GLFW_PRESS) {
					select(world);
				}
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
				} else if (world.getY() >= pos.getY() - MOVE_SIZE - SELECT_RADIUS && world.getY() <= pos.getY() - MOVE_SIZE + SELECT_RADIUS) {
					moveDir = Vec2D(0, -1);
					moving = true;
				}
			}
			//Possible horizontal match
			else if (world.getY() >= pos.getY() - SELECT_RADIUS && world.getY() <= pos.getY() + SELECT_RADIUS) {
				if (world.getX() >= pos.getX() + MOVE_SIZE - SELECT_RADIUS && world.getX() <= pos.getX() + MOVE_SIZE + SELECT_RADIUS) {
					moveDir = Vec2D(1, 0);
					moving = true;
				} else if (world.getX() >= pos.getX() - MOVE_SIZE - SELECT_RADIUS && world.getX() <= pos.getX() - MOVE_SIZE + SELECT_RADIUS) {
					moveDir = Vec2D(-1, 0);
					moving = true;
				}
			}
			if (!moving) {
				select(world);
			}
			break;
		}
		case 2: //Resize
		{
			//If the current object cannot be resized or doesn't exist
			if (!selected || !selected->canResize()) {
				if (action == GLFW_PRESS) {
					select(world);
				}
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
			if (!resizing) {
				select(world);
			}
			break;
		}
		case 3: //Rotate
		{
			//If the current object can't be rotated or doesn't exist
			if (!selected || !selected->canRotate()) {
				if (action == GLFW_PRESS) {
					select(world);
				}
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
			} else {
				select(world);
			}
			break;
		}
		case 4: //Options
		{
			if (action == GLFW_PRESS) {
				select(world);
			}
			if (selected) {
				selected->createOptions();
				optMenu = selected->getOptions();
				currentMenu = Menu::OPTIONS;
			}
			break;
		}
		case 5: //Delete
		{
			//Deselect current object in case it is removed
			if (selected) {
				selected = NULL;
			}
			//Prevent accidently clicking on something
			if (action != GLFW_RELEASE) {
				break;
			}
			//Iterate through and remove those that are being clicked on
			auto gravIt = gravFields.begin();
			while (gravIt != gravFields.end()) {
				if ((*gravIt)->isInBoundingBox(world.getX(), world.getY()) && (*gravIt)->canDelete()) {
					delete *gravIt;
					gravIt = gravFields.erase(gravIt);
				} else {
					gravIt++;
				}
			}
			auto platIt = platforms.begin();
			while (platIt != platforms.end()) {
				if ((*platIt)->isInBoundingBox(world.getX(), world.getY()) && (*platIt)->canDelete()) {
					delete *platIt;
					platIt = platforms.erase(platIt);
				} else {
					platIt++;
				}
			}
			break;
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
		} else {
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
		} else {
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


void LevelEditor::textEvent(GLFWwindow *, unsigned int ch) {
	//Pass text input to textboxes
	if (currentMenu == Menu::SAVE) {
		fileBox.textEvent(ch);
		levelBox.textEvent(ch);
		nextBox.textEvent(ch);
		gravBox.textEvent(ch);
	} else if (currentMenu == Menu::OPTIONS) {
		optMenu->textEvent(ch);
	}
}


// Sets the current menu
void LevelEditor::setMenu(int m) {
	currentMenu = (Menu)m;
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


// Gets the file location to save the level to
string LevelEditor::getFileLocation() {
	return "Levels\\" + fileBox.getText();
}


// Saves the level to the given file
void LevelEditor::saveLevel(string filePath) {
	//TODO: Checks for valid file location/name etc
	//Remove spawn and goal objects
	auto platIt = platforms.begin();
	while (platIt != platforms.end()) {
		string id = (*platIt)->getId();
		if (id == "spawn" || id == "goal") {
			delete *platIt;
			platIt = platforms.erase(platIt);
		} else {
			platIt++;
		}
	}
	//Update settings
	levelName = levelBox.getText();
	defaultGravity = atof(gravBox.getText().c_str());
	nextLevelPath = nextBox.getText();
	LevelRenderer::saveLevel(filePath);
	//Close menu
	currentMenu = Menu::NONE;
	//Re-add spawn+goal objects
	SpawnPoint* spawnObj = new SpawnPoint();
	spawnObj->setPos(spawn);
	spawnObj->setAngle(spawnAngle);
	platforms.push_back(spawnObj);
	Goal* goalObj = new Goal();
	goalObj->setPos(goal);
	goalObj->setAngle(goalAngle);
	platforms.push_back(goalObj);

}


// Loads a level from the given file
void LevelEditor::loadLevel(string filePath) {
	//TODO: Checks for valid path
	LevelRenderer::loadLevel(filePath);
	//Close menu
	currentMenu = Menu::NONE;
	SpawnPoint* spawnObj = new SpawnPoint();
	spawnObj->setPos(spawn);
	spawnObj->setAngle(spawnAngle);
	platforms.push_back(spawnObj);
	Goal* goalObj = new Goal();
	goalObj->setPos(goal);
	goalObj->setAngle(goalAngle);
	platforms.push_back(goalObj);
	//Update menu
	nextBox.setText(nextLevelPath);
	levelBox.setText(levelName);
	gravBox.setText(to_string(defaultGravity));
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
	//Rotate movement to match camera angle
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


// Selects a selectable at position "world"
void LevelEditor::select(Vec2D world) {
	//By breaking after finding a match different types of object have different priorities
	selected = NULL;
	//TODO: Entities
	for (Entity* e : entities) {
		if (e->isInBoundingBox(world.getX(), world.getY())) {
			selected = e;
			return;
		}
	}
	for (Platform* p : platforms) {
		if (p->isInBoundingBox(world.getX(), world.getY())) {
			selected = p;
			return;
		}
	}
	for (GravityField* f : gravFields) {
		if (f->isInBoundingBox(world.getX(), world.getY())) {
			selected = f;
			return;
		}
	}
}


// Draws a TexBox with a corresponding label
void LevelEditor::drawTextBox(string label, TextBox &box, int y) {
	int textWidth = freetype::getLength(fontSmall, (label + " ").c_str());
	glColor3ub(0, 0, 0);
	freetype::print(fontSmall, sWidth * 0.1f, y - fontSmall.h * 0.325f, label.c_str());
	box.setWidth((4 * sWidth) / 5 - textWidth);
	box.setHeight((int)(fontSmall.h * 1.5f));
	box.setX((sWidth + textWidth) / 2);
	box.setY(y);
	box.draw();
}

void LevelEditor::drawHandle(double x, double y) {
	glBegin(GL_QUADS);
	glVertex2d(x - SELECT_RADIUS, y - SELECT_RADIUS);
	glVertex2d(x + SELECT_RADIUS, y - SELECT_RADIUS);
	glVertex2d(x + SELECT_RADIUS, y + SELECT_RADIUS);
	glVertex2d(x - SELECT_RADIUS, y + SELECT_RADIUS);
	glEnd();
}


// Adds a platform to the level
void LevelEditor::addPlatform(Platform* platform) {
	platforms.push_back(platform);
}


// Adds a platform to the level
void LevelEditor::addGravityField(GravityField* field) {
	gravFields.push_back(field);
}
