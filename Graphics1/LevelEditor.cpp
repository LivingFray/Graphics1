#include "LevelEditor.h"
#include "MainMenu.h"
#include "GradButton.h"
#include "SpawnPoint.h"
#include "Goal.h"
#include "PointGiver.h"
#include "BombEntity.h"
#include "ShieldGiver.h"
#include "Spike.h"
#include "StompableEntity.h"
#include "MovingPlatform.h"
#include "TextItem.h"
#include "Slope.h"
#include "Turret.h"
#include "BreakablePlatform.h"
#include "Lever.h"
#include "ANDGate.h"
#include "NANDGate.h"
#include "ORGate.h"
#include "NORGate.h"
#include "XORGate.h"
#include "XNORGate.h"
#include "PhasingPlatform.h"
#include "ButtonEntity.h"
#include "Crate.h"
#include "Clock.h"
#include "TimedButton.h"
#include "Light.h"
#define EDITOR_MOVE_SPEED 1.00
#define EDITOR_ROTATE_SPEED 30
#define MOVE_SIZE 0.5
#define SELECT_RADIUS 0.1
#define POS_SNAP 0.05
#define ANG_SNAP 0.05
#define ROTATE_SEGMENTS 36
//Make the compiler shut up about missing parameters in the platform macro
#pragma warning(disable:4003)

#define ADD_ENTITY(type, e_name) 	item = MenuItem(); \
item.name = e_name; \
item.create = [](BaseState* s) { \
	LevelEditor* l = (LevelEditor*)s; \
	type* e = new type(); \
	e->setPos(l->getCameraPos()); \
	e->setAngle(l->getCameraAngleAt(0)); \
	e->setEditor(); \
	l->addEntity(e); \
	l->setMenu(0); \
}; \
menuItems.push_back(item);

#define ADD_PLATFORM(type, p_name, extra)	item = MenuItem(); \
item.name = p_name; \
item.create = [](BaseState* s) { \
	LevelEditor* l = (LevelEditor*)s; \
	type* pl = new type(); \
	pl->setPos(l->getCameraPos()); \
	if (pl->canResize()){ \
		pl->setWidth(1.00); \
		pl->setHeight(1.00); \
	} \
	pl->setEditor(); \
	extra; \
	pl->setAngle(l->getCameraAngleAt(0)); \
	l->addPlatform(pl); \
	l->setMenu(0); \
}; \
menuItems.push_back(item);

#define ADD_SCENERY(type, s_name)	item = MenuItem(); \
item.name = s_name; \
item.create = [](BaseState* s) { \
	LevelEditor* l = (LevelEditor*)s; \
	type* sc = new type(); \
	sc->setPos(l->getCameraPos()); \
	if (sc->canResize()){ \
		sc->setWidth(1.00); \
		sc->setHeight(1.00); \
	} \
	sc->setAngle(l->getCameraAngleAt(0)); \
	sc->setEditor(); \
	l->addScenery(sc); \
	l->setMenu(0); \
}; \
menuItems.push_back(item);

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
	optMenu = NULL;
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
	barButtons[6] = ImageLoader::getImage("Resources\\buttons\\copy.png");
	//Create elements of spawn menu
	MenuItem item;
	item = MenuItem();
	item.name = "Gravity Field";
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
	ADD_PLATFORM(Platform, "Basic Platform");
	ADD_PLATFORM(Spike, "Deadly Platform");
	ADD_PLATFORM(BreakablePlatform, "Breakable Platform");
	ADD_PLATFORM(MovingPlatform, "Moving Platform", pl->setFirstNode(l->getCameraPos()); pl->setLastNode(l->getCameraPos()));
	ADD_PLATFORM(Slope, "Slope");
	ADD_PLATFORM(PhasingPlatform, "Phasing Platform");
	ADD_ENTITY(PointGiver, "Point Giver");
	ADD_ENTITY(BombEntity, "Bomb");
	ADD_ENTITY(StompableEntity, "Stompable");
	ADD_ENTITY(ShieldGiver, "Shield Giver");
	ADD_ENTITY(Turret, "Turret");
	ADD_ENTITY(ButtonEntity, "Button");
	ADD_ENTITY(Crate, "Crate");
	ADD_SCENERY(Lever, "Lever");
	ADD_SCENERY(TextItem, "Text Box");
	ADD_SCENERY(ANDGate, "AND Gate");
	ADD_SCENERY(ORGate, "OR Gate");
	ADD_SCENERY(XORGate, "XOR Gate");
	ADD_SCENERY(NANDGate, "NAND Gate");
	ADD_SCENERY(NORGate, "NOR Gate");
	ADD_SCENERY(XNORGate, "XNOR Gate");
	ADD_SCENERY(Clock, "Clock");
	ADD_SCENERY(TimedButton, "Timed Button");
	ADD_SCENERY(Light, "Light");
	//Create buttons for menu
	for (MenuItem i : menuItems) {
		Button* b = new Button();
		b->setLabel(i.name);
		b->setCallback(i.create);
		buttons.push_back(b);
	}
	//Initialise the text boxes used on the settings screen
	fileBox = TextBox();
	fileBox.setText("");
	textBoxes.push_back(&fileBox);
	levelBox = TextBox();
	levelBox.setText("");
	textBoxes.push_back(&levelBox);
	nextBox = TextBox();
	nextBox.setText("");
	textBoxes.push_back(&nextBox);
	gravBox = TextBox();
	gravBox.setText("");
	gravBox.setNumeric(true);
	textBoxes.push_back(&gravBox);
	targetBox = TextBox();
	targetBox.setText("");
	targetBox.setNumeric(true);
	textBoxes.push_back(&targetBox);
	goalBox = TextBox();
	goalBox.setText("");
	goalBox.setNumeric(true);
	textBoxes.push_back(&goalBox);
	//Initialise menu buttons
	exitButton = GradButton();
	exitButton.setLabel("Exit to menu");
	auto exitCall = [](BaseState* s) {
		newState = new MainMenu();
	};
	exitButton.setCallback(exitCall);
	returnButton = GradButton();
	returnButton.setLabel("Return to level");
	auto returnCall = [](BaseState* s) {
		LevelEditor* l = (LevelEditor*)s;
		l->setMenu((int)Menu::NONE);
	};
	returnButton.setCallback(returnCall);
	loadLevel("");
}


LevelEditor::~LevelEditor() {
	glDeleteLists(rotateCall, 1);
	for (Button* b : buttons) {
		delete b;
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
	LevelBase::draw(0);
	//Draw movement arrows/resize arrows
	if (selected) {
		//Draw point showing centre of object
		Vec2D pos = selected->getPos();
		glColor3ub(255, 127, 0);
		glLineWidth(4);
		drawHandle(pos.getX(), pos.getY());
		switch (current) {
		case 1: //Move
			if (selected->canMove()) {
				drawMovingHandles(pos);
			}
			break;
		case 2: //Resize
		{
			if (selected->canResize()) {
				drawResizingHandles(pos);
			}
			break;
		}
		case 3: //Rotate
			if (selected->canRotate()) {
				drawRotatingHandles(pos);
			}
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
		drawItemMenu(ex);
	} else if (currentMenu == Menu::SAVE) {
		drawSaveMenu(ex);
	} else if (currentMenu == Menu::OPTIONS) {
		drawOptionsMenu(ex);
	} else {
		drawIngameUI(ex);
	}
}


void LevelEditor::keyEvent(GLFWwindow * window, int key, int scan, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		if (currentMenu != Menu::NONE) {
			currentMenu = Menu::NONE;
			if (optMenu) {
				optMenu = NULL;
			}
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
		for (TextBox* t : textBoxes) {
			t->keyDown(key, scan, action, mods);
		}
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
			for (Button* b : buttons) {
				b->mouseDown((int)x, (int)y);
			}
		}
	} else if (currentMenu == Menu::SAVE) {
		if (action == GLFW_RELEASE) {
			int ix = (int)x;
			int iy = (int)y;
			loadButton.mouseDown(ix, iy);
			saveButton.mouseDown(ix, iy);
			exitButton.mouseDown(ix, iy);
			returnButton.mouseDown(ix, iy);
			for (TextBox* t : textBoxes) {
				t->mouseDown(ix, iy);
			}
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
		clickHandles(x, y, action);
	}
}


// Called when a mouse move event is fired
void LevelEditor::mouseMoveEvent(GLFWwindow* window, double x, double y) {
	//The camera is being moved
	if (panning) {
		double scale = WORLD_SIZE / (double)(sWidth < sHeight ? sWidth : sHeight);
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
	//A selectable is being dragged
	if (dragging) {
		if (currentMenu != Menu::NONE) {
			dragging = false;
		} else {
			Vec2D world = getWorldCoordinates(Vec2D(x, sHeight - y));
			Vec2D dif = world.subtract(dragFrom);
			selected->onMove(dif.getX(), dif.getY());
			dragFrom = world;
		}
	}
}


void LevelEditor::textEvent(GLFWwindow *, unsigned int ch) {
	//Pass text input to textboxes
	if (currentMenu == Menu::SAVE) {
		for (TextBox* t : textBoxes) {
			t->textEvent(ch);
		}
	} else if (currentMenu == Menu::OPTIONS) {
		optMenu->textEvent(ch);
	}
}


void LevelEditor::resizeEvent(GLFWwindow * window, int width, int height) {
	int i = 0;
	//Resize item menu
	for (Button* b : buttons) {
		b->setWidth((4 * sWidth) / 10);
		b->setHeight((int)(fontLarge.h * 1.75));
		b->setX(sWidth / 4 + (i % 2) * (sWidth / 2));
		b->setY(sHeight - (int)fontLarge.h * 2.5 - (int)(fontLarge.h * 1.75) * (i / 2));
		i++;
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
	targetTime = atof(targetBox.getText().c_str());
	nextLevelPath = nextBox.getText();
	goalChannel = atoi(goalBox.getText().c_str());
	LevelBase::saveLevel(filePath);
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
	LevelBase::loadLevel(filePath);
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
	targetBox.setText(to_string(targetTime));
	goalBox.setText(to_string(goalChannel));
	for (Entity* e : entities) {
		e->setEditor();
	}
	for (Platform* p : platforms) {
		p->setEditor();
	}
	for (Scenery* s : scenery) {
		s->setEditor();
	}
	for (GravityField* g : gravFields) {
		g->setEditor();
	}
}


// Updates the camera position and rotation
void LevelEditor::updateCamera(double time) {
	// Don't move camera in menus
	if (currentMenu != Menu::NONE) {
		return;
	}
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
	for (Scenery* s : scenery) {
		if (s->isInBoundingBox(world.getX(), world.getY())) {
			selected = s;
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


// Draws a TextBox with a corresponding label
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


// Draws a smaller TextBox with a corresponding label
void LevelEditor::drawSmallTextBox(string label, TextBox &box, bool left, int y) {
	int textWidth = freetype::getLength(fontSmall, (label + " ").c_str());
	glColor3ub(0, 0, 0);
	freetype::print(fontSmall, (left ? 4 : 21) * sWidth / 40.0f, y - fontSmall.h * 0.325f, label.c_str());
	box.setWidth((15 * sWidth) / 40 - textWidth);
	box.setHeight((int)(fontSmall.h * 1.5f));
	if (left) {
		box.setX((sWidth / 10 + textWidth + 19 * sWidth / 40) / 2);
	} else {
		box.setX(((21 * sWidth) / 40 + textWidth + 18 * sWidth / 20) / 2);
	}
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


void inline LevelEditor::drawMovingHandles(Vec2D pos) {
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
}


void inline LevelEditor::drawResizingHandles(Vec2D pos) {
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
}


void inline LevelEditor::drawRotatingHandles(Vec2D pos) {
	glPushMatrix();
	glTranslated(pos.getX(), pos.getY(), 0.0);
	glColor3ub(0, 127, 0);
	glCallList(rotateCall);
	glPopMatrix();
}


void inline LevelEditor::drawItemMenu(double ex) {
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
	for (Button* b : buttons) {
		b->draw(ex);
	}
}


void inline LevelEditor::drawSaveMenu(double ex) {
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
	drawSmallTextBox("Default gravity", gravBox, true, y);
	//Target time
	drawSmallTextBox("Target time", targetBox, false, y);
	//Next level
	y -= (int)fontSmall.h * 4;
	drawTextBox("Next level", nextBox, y);
	//Goal channel
	y -= (int)fontSmall.h * 4;
	drawSmallTextBox("Goal channel", goalBox, true, y);
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
}


void inline LevelEditor::drawOptionsMenu(double ex) {
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
}


void inline LevelEditor::drawIngameUI(double ex) {
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
	glColor3ub(0, 0, 0);
	freetype::print(fontSmall, 10, fontSmall.h * 2.625f, "Position: (%.2f, %.2f)\nAngle: %.2f", camPos.getX(), camPos.getY(), camAngle);
}


void inline LevelEditor::clickHandles(double x, double y, int action) {
	//Convert coords to world coords
	Vec2D world = getWorldCoordinates(Vec2D(x, y));
	switch (current) {
	case 0: //Select
		dragClicked(world, action);
	case 1: //Move
		moveClicked(world, action);
		break;
	case 2: //Resize
		resizeClicked(world, action);
		break;
	case 3: //Rotate
		rotateClicked(world, action);
		break;
	case 4: //Options
		if (action == GLFW_PRESS) {
			select(world);
		}
		if (selected) {
			selected->createOptions();
			optMenu = selected->getOptions();
			currentMenu = Menu::OPTIONS;
		}
		break;
	case 5: //Delete
		deleteClicked(world, action);
		break;
	case 6: //Copy
		copyClicked(world, action);
		break;
	}
}


void inline LevelEditor::dragClicked(Vec2D world, int action) {
	if (action == GLFW_PRESS) {
		select(world);
		if (selected) {
			dragFrom = world;
			dragging = true;
		}
	} else {
		dragging = false;
	}
}

void inline LevelEditor::moveClicked(Vec2D world, int action) {
	//If the current object cannot be moved or doesn't exist
	if (!selected || !selected->canMove()) {
		if (action == GLFW_PRESS) {
			select(world);
		}
		return;
	}
	//Stop moving the object
	if (action == GLFW_RELEASE) {
		moving = false;
		return;
	}
	Vec2D pos = selected->getPos();
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
}


void LevelEditor::resizeClicked(Vec2D world, int action) {
	//If the current object cannot be resized or doesn't exist
	if (!selected || !selected->canResize()) {
		if (action == GLFW_PRESS) {
			select(world);
		}
		return;
	}
	//Stop resizing the object
	if (action == GLFW_RELEASE) {
		resizing = 0;
		return;
	}
	//Determine vectors handles lie upon
	Vec2D width = Vec2D(cos(selected->getAngle() * DEG_TO_RAD), sin(selected->getAngle() * DEG_TO_RAD));
	Vec2D height = Vec2D(-width.getY(), width.getX());
	width.multiplyBy(selected->getWidth() * 0.5);
	height.multiplyBy(selected->getHeight() * 0.5);
	//Check each handle to see if it is being clicked on
	Vec2D handle = selected->getPos().add(width);
	if (world.getX() >= handle.getX() - SELECT_RADIUS && world.getX() <= handle.getX() + SELECT_RADIUS
		&& world.getY() >= handle.getY() - SELECT_RADIUS && world.getY() <= handle.getY() + SELECT_RADIUS) {
		scaleDir = width;
		resizing = 1;
		return;
	}
	handle = selected->getPos().subtract(width);
	if (world.getX() >= handle.getX() - SELECT_RADIUS && world.getX() <= handle.getX() + SELECT_RADIUS
		&& world.getY() >= handle.getY() - SELECT_RADIUS && world.getY() <= handle.getY() + SELECT_RADIUS) {
		scaleDir = width.multiply(-1);
		resizing = 2;
		return;
	}
	handle = selected->getPos().add(height);
	if (world.getX() >= handle.getX() - SELECT_RADIUS && world.getX() <= handle.getX() + SELECT_RADIUS
		&& world.getY() >= handle.getY() - SELECT_RADIUS && world.getY() <= handle.getY() + SELECT_RADIUS) {
		scaleDir = height;
		resizing = 3;
		return;
	}
	handle = selected->getPos().subtract(height);
	if (world.getX() >= handle.getX() - SELECT_RADIUS && world.getX() <= handle.getX() + SELECT_RADIUS
		&& world.getY() >= handle.getY() - SELECT_RADIUS && world.getY() <= handle.getY() + SELECT_RADIUS) {
		scaleDir = height.multiply(-1);
		resizing = 4;
		return;
	}
	if (!resizing) {
		select(world);
	}
}


void inline LevelEditor::rotateClicked(Vec2D world, int action) {
	//If the current object can't be rotated or doesn't exist
	if (!selected || !selected->canRotate()) {
		if (action == GLFW_PRESS) {
			select(world);
		}
		return;
	}
	//Stop rotating the object
	if (action == GLFW_RELEASE) {
		rotating = false;
		return;
	}
	double d = world.subtract(selected->getPos()).magnitudeSquare();
	double min = MOVE_SIZE - SELECT_RADIUS;
	double max = MOVE_SIZE + SELECT_RADIUS;
	if (d >= min * min && d <= max * max) {
		rotating = true;
		Vec2D dif = world.subtract(selected->getPos());
		rotateFrom = atan2(dif.getY(), dif.getX());
	} else {
		select(world);
	}
}


void inline LevelEditor::deleteClicked(Vec2D world, int action) {
	//Deselect current object in case it is removed
	if (selected) {
		selected = NULL;
	}
	//Prevent accidently clicking on something
	if (action != GLFW_RELEASE) {
		return;
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
	auto scenIt = scenery.begin();
	while (scenIt != scenery.end()) {
		if ((*scenIt)->isInBoundingBox(world.getX(), world.getY()) && (*scenIt)->canDelete()) {
			delete *scenIt;
			scenIt = scenery.erase(scenIt);
		} else {
			scenIt++;
		}
	}
	auto entIt = entities.begin();
	while (entIt != entities.end()) {
		if ((*entIt)->isInBoundingBox(world.getX(), world.getY()) && (*entIt)->canDelete()) {
			delete *entIt;
			entIt = entities.erase(entIt);
		} else {
			entIt++;
		}
	}
}


void inline LevelEditor::copyClicked(Vec2D world, int action) {
	//Don't copy if nothing selected
	if (!selected || action != GLFW_PRESS) {
		return;
	}
	//Iterate through and copy those that are being clicked on
	auto entIt = entities.begin();
	while (entIt != entities.end()) {
		if ((*entIt)->isInBoundingBox(world.getX(), world.getY()) && (*entIt)->canDelete()) {
			Entity* e = new Entity(**entIt);
			selected = e;
			current = 0;
			dragClicked(world, action);
			entities.push_back(e);
			return;
		} else {
			entIt++;
		}
	}
	auto platIt = platforms.begin();
	while (platIt != platforms.end()) {
		if ((*platIt)->isInBoundingBox(world.getX(), world.getY()) && (*platIt)->canDelete()) {
			Platform* p = new Platform(**platIt);
			selected = p;
			current = 0;
			dragClicked(world, action);
			platforms.push_back(p);
			return;
		} else {
			platIt++;
		}
	}
	auto scenIt = scenery.begin();
	while (scenIt != scenery.end()) {
		if ((*scenIt)->isInBoundingBox(world.getX(), world.getY()) && (*scenIt)->canDelete()) {
			Scenery* s = new Scenery(**scenIt);
			selected = s;
			current = 0;
			dragClicked(world, action);
			scenery.push_back(s);
			return;
		} else {
			scenIt++;
		}
	}
	auto gravIt = gravFields.begin();
	while (gravIt != gravFields.end()) {
		if ((*gravIt)->isInBoundingBox(world.getX(), world.getY()) && (*gravIt)->canCopy()) {
			GravityField* g = new GravityField(**gravIt);
			selected = g;
			current = 0;
			dragClicked(world, action);
			gravFields.push_back(g);
			return;
		} else {
			gravIt++;
		}
	}
}
