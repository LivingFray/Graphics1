#include "Level.h"
#include "Globals.h"
#include "Player.h"
#include "MainMenu.h"
#include "Collision.h"


Level::Level() {
	//Load textures
	spawnBeam = ImageLoader::getImage("Resources\\spawnBeam.png");
	//Load animation
	spawnAnim = Animation();
	spawnAnim.setPos(Vec2D(0, 0));
	spawnAnim.setSpriteSheet("Resources\\spawnDoors.png");
	spawnAnim.setRepeat(false);
	spawnAnim.setSpritesheetSize(4, 2);
	spawnAnim.setSize(SPAWN_WIDTH, SPAWN_HEIGHT);
	for (int i = 0; i < 8; i++) {
		spawnAnim.addFrame(i, (SPAWN_ANIM_END - SPAWN_ANIM_DOORS) / 8);
	}
	spawnAnim.setTime(0);
	//Reset variables
	levelTime = 0;
	reachedGoal = false;
	paused = false;
	failed = false;
	score = 0;
	nextLevelPath = "";
	//Create menus
	buttonMenu = Button();
	buttonMenu.setLabel("Menu");
	auto callMenu = [](BaseState* s) {
		newState = new MainMenu();
	};
	buttonMenu.setCallback(callMenu);
	buttonNext = Button();
	buttonNext.setLabel("Next Level");
	auto callNext = [](BaseState* s) {
		Level* l = (Level*)s;
		l->nextLevel();
	};
	buttonNext.setCallback(callNext);
	buttonRetry = Button();
	buttonRetry.setLabel("Retry");
	auto callRetry = [](BaseState* s) {
		Level* l = (Level*)s;
		l->restartLevel();
	};
	buttonRetry.setCallback(callRetry);
	gradMenu = GradButton();
	gradMenu.setLabel("Main Menu");
	gradMenu.setCallback(callMenu);
	gradResume = GradButton();
	gradResume.setLabel("Resume");
	auto callResume = [](BaseState* s) {
		Level* l = (Level*)s;
		l->setPause(false);
	};
	gradResume.setCallback(callResume);
	gradRetry = GradButton();
	gradRetry.setLabel("Restart Level");
	gradRetry.setCallback(callRetry);
	//Load audio
	spawnSound = SoundLoader::getSound("Resources\\sounds\\spawn.wav");
	goalSound = SoundLoader::getSound("Resources\\sounds\\goal.wav");
	//Create channels
	channels = new char[MAX_CHANNELS / 8]();
	newChannels = new char[MAX_CHANNELS / 8]();
}


Level::~Level() {
	alDeleteSources(1, &spawnSound);
	delete[] channels;
	delete[] newChannels;
}


// Updates the level
void Level::update() {
	//Don't update in menus
	if (reachedGoal) {
		goalTime += TICKRATE;
	}
	if (reachedGoal || paused || failed) {
		return;
	}
	if (levelTime == 0) {
		alSourcePlay(spawnSound);
	}
	//Add player if finished spawning
	if (!player && levelTime >= SPAWN_ANIM_BEGIN && !reachedGoal) {
		player = new Player();
		entities.push_back(player);
		player->setPos(spawn);
		player->setAngle(spawnAngle);
		player->setVisAngle(spawnAngle);
	}
	//Update animation
	if (levelTime >= SPAWN_ANIM_DOORS) {
		spawnAnim.addTime(TICKRATE);
	}
	//Don't update if the spawn animation is still ongoing
	if (levelTime <= SPAWN_ANIM_END) {
		levelTime += TICKRATE;
		return;
	}
	//Reset new channel
	delete[] newChannels;
	newChannels = new char[MAX_CHANNELS / 8]();
	//Update gravity fields
	for (GravityField* g : gravFields) {
		g->update();
	}
	for (Scenery* s : scenery) {
		s->update();
	}
	//Update the platforms
	for (Platform* p : platforms) {
		p->update();
	}
	//Update the entities
	for (unsigned int i = 0; i < entities.size(); i++) {
		Entity* e = entities.at(i);
		e->update();
		bool onGround = e->getOnGround();
		if (e->getVel().magnitudeSquare() > FLOAT_ZERO) {
			onGround = false;
		}
		Vec2D res;
		//Start by checking the entity after 'e'
		auto start = entities.begin() + i + 1;
		while (start != entities.end()) {
			Entity* e2 = *start;
			//Entities don't collide with each other
			if (Collision::intersects(e, e2, &res)) {
				e->onCollide(e2);
				e2->onCollide(e);
			}
			start++;
		}
		//Perform collision detection + resolution
		for (Platform* p : platforms) {
			Collision::handle(this, e, p, onGround);
		}
		e->setOnGround(onGround);
	}
	//Set whether the goal is active
	goalOpen = getChannel(goalChannel);
	checkPlayerReachedGoal();
	//Add and delete objects that were requested to be added/deleted
	handleChangedObjects();
	//Reset channels
	std::swap(newChannels, channels);
	//Level timer
	levelTime += TICKRATE;
}


// Loads a level from the given file
void Level::loadLevel(string filePath) {
	LevelBase::loadLevel(filePath);
	levelTime = 0;
	spawnAnim.setTime(0);
	reachedGoal = false;
	goalTime = 0;
	paused = false;
	failed = false;
	score = 0;
	player = NULL;
	calculateBounds();
}


// Draws the level
void Level::draw(double ex) {
	if (reachedGoal || paused || failed || levelTime < SPAWN_ANIM_END) {
		//Don't extrapolate if the game is paused
		LevelBase::draw(0);
	} else {
		LevelBase::draw(ex);
	}
	//Spawn animation
	glPushMatrix();
	glTranslated(spawn.getX(), spawn.getY(), 0.0);
	glRotated(spawnAngle, 0.0, 0.0, 1.0);
	if (levelTime <= SPAWN_ANIM_END) {
		//Set opacity of spawn beam
		if (levelTime >= SPAWN_ANIM_BEGIN) {
			glColor4d(1.0, 1.0, 1.0, 1.25 * (1.0 - pow(2.25 * (levelTime - SPAWN_ANIM_BEGIN) / (SPAWN_ANIM_END - SPAWN_ANIM_BEGIN), 0.5)) - 0.25);
		} else {
			glColor4d(1.0, 1.0, 1.0, 1.0);
		}
		drawBeam();
	}
	//Draw doors
	spawnAnim.draw(ex);
	glPopMatrix();
	//Goal animation
	if (reachedGoal && goalTime < GOAL_ANIM_END) {
		glPushMatrix();
		//Set opacity for goal beam
		glColor4d(1.0, 1.0, 1.0, 1.25 * (1.0 - pow(2.25 * (goalTime) / GOAL_ANIM_END, 0.5)) - 0.25);
		glTranslated(goal.getX(), goal.getY(), 0.0);
		glRotated(goalAngle, 0.0, 0.0, 1.0);
		drawBeam();
		glPopMatrix();
	}
	//Reset viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, sWidth, 0.0, sHeight, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//Different possible UIs
	if ((reachedGoal && goalTime >= GOAL_ANIM_END) || failed) {
		drawCompletedScreen();
	} else if (paused) {
		drawPauseScreen();
	} else {
		drawIngameUI();
		drawMinimap(ex);
	}
}


void inline Level::drawBeam() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spawnBeam);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(-SPAWN_WIDTH * 0.5, -SPAWN_HEIGHT * 0.5);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(-SPAWN_WIDTH * 0.5, SPAWN_HEIGHT * 0.5);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(SPAWN_WIDTH * 0.5, SPAWN_HEIGHT * 0.5);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(SPAWN_WIDTH * 0.5, -SPAWN_HEIGHT * 0.5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


void inline Level::drawCompletedScreen() {
	//Background
	glColor4ub(0, 127, 255, 63);
	glBegin(GL_QUADS);
	glVertex2d(sWidth * 0.125, sHeight * 0.375);
	glVertex2d(sWidth * 0.875, sHeight * 0.375);
	glVertex2d(sWidth * 0.875, sHeight * 0.625);
	glVertex2d(sWidth * 0.125, sHeight * 0.625);
	glEnd();
	//Title
	glColor3ub(255, 255, 255);
	if (reachedGoal) {
		printCentre(fontLarge, sWidth * 0.5, sHeight * 0.625 - fontLarge.h * 1.5, "Level Complete");
	} else {
		printCentre(fontLarge, sWidth * 0.5, sHeight * 0.625 - fontLarge.h * 1.5, "Level failed");
	}
	//Score
	string scoreLabel = "Score: ";
	scoreLabel += to_string(score);
	printCentre(fontLarge, sWidth * 0.5, sHeight * 0.625 - fontLarge.h * 3, scoreLabel.c_str());
	//Buttons
	int y = (int)(sHeight * 0.375) + (int)fontLarge.h;
	int h = (int)fontLarge.h * 2;
	//Make buttons take up whole of the window
	float move = failed ? (1 / 3.0f) : 0.25f;
	int w = (int)(sWidth * move);
	//Set position of retry and menu buttons
	buttonRetry.setX((int)(sWidth * move));
	buttonRetry.setY(y);
	buttonRetry.setWidth(w);
	buttonRetry.setHeight(h);
	buttonRetry.draw();
	buttonMenu.setX((int)(sWidth * move * 2));
	buttonMenu.setY(y);
	buttonMenu.setWidth(w);
	buttonMenu.setHeight(h);
	buttonMenu.draw();
	if (reachedGoal) {
		//Set position of next level button
		buttonNext.setX((int)(sWidth * move * 3));
		buttonNext.setY(y);
		buttonNext.setWidth(w);
		buttonNext.setHeight(h);
		buttonNext.draw();
	}
}


void inline Level::drawPauseScreen() {
	//Darken the screen to show the game is paused
	glColor4ub(0, 0, 0, 150);
	glBegin(GL_QUADS);
	glVertex2i(0, 0);
	glVertex2i(sWidth, 0);
	glVertex2i(sWidth, sHeight);
	glVertex2i(0, sHeight);
	glEnd();
	//Draw the buttons
	int y = (int)(fontLarge.h * 2.1);
	int w = sWidth / 2;
	int h = (int)fontLarge.h * 2;
	int top = h * 3;
	//Set position of buttons
	gradResume.setY(top);
	gradRetry.setY(top - y);
	gradMenu.setY(top - 2 * y);
	gradResume.setHeight(h);
	gradRetry.setHeight(h);
	gradMenu.setHeight(h);
	gradResume.setWidth(w - 2 * y);
	gradRetry.setWidth(w - y);
	gradMenu.setWidth(w);
	gradResume.draw(0);
	gradRetry.draw(0);
	gradMenu.draw(0);
}


void inline Level::drawIngameUI() {
	glColor3ub(0, 0, 0);
	double remaining = targetTime - levelTime;
	//Switch score colour to show time is up
	if (remaining <= 0) {
		glColor3ub(255, 0, 0);
		remaining *= -1;
	}
	freetype::print(fontSmall, 10.0f, sHeight - fontSmall.h, "Score: %d Time: %d:%02d", score, (int)remaining / 60, (int)remaining % 60);
#ifdef DEBUG
	//Display player position
	glColor3ub(0, 0, 0);
	if (player) {
		freetype::print(fontSmall, 10.0f, sHeight - fontSmall.h * 3, "Position: %2.2f, %2.2f", player->getPos().getX(), player->getPos().getY());
	}
	//Display current channels
#define display 32
	char out[display + 1];
	for (int i = 0; i < display; i++) {
		out[i] = getChannel(i) ? '1':'0';
	}
	out[display] = '\0';
	freetype::print(fontSmall, 10.0f, sHeight - fontSmall.h * 5, out);
#endif
}


void inline Level::drawMinimap(double ex) {
	int s = (int)((sWidth > sHeight ? sHeight : sWidth) * MINIMAP_SIZE);
	//Outline
	glColor3ub(0, 0, 0);
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
	glVertex2d(sWidth, sHeight);
	glVertex2d(sWidth - s, sHeight);
	glVertex2d(sWidth - s, sHeight - s);
	glVertex2d(sWidth, sHeight - s);
	glEnd();
	//Switch scale so only minimap is drawn
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Scale it such that I can draw 1 to 1
	float maxDir = (mapMaxX - mapMinX) > (mapMaxY - mapMinY) ? (mapMaxX - mapMinX) : (mapMaxY - mapMinY);
	float resize = (float)(maxDir/MINIMAP_SIZE) / (float)(sWidth < sHeight ? sWidth : sHeight);
	glOrtho(0.0, resize * sWidth, 0.0, resize * sHeight, -1.0, 1.0);
	glEnable(GL_SCISSOR_TEST);
	glScissor((1 - MINIMAP_SIZE) * sWidth, (1 - MINIMAP_SIZE) * sHeight, MINIMAP_SIZE * sWidth, MINIMAP_SIZE * sHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3ub(0, 255, 0);
	glTranslated(resize * sWidth - maxDir - mapMinX, resize * sHeight - maxDir - mapMinY, 0);
	glColor3ub(0, 0, 0);
	for (Platform* p : platforms) {
		int v;
		Vec2D* vert = p->getVertices(&v);
		glBegin(GL_POLYGON);
		for (int i = 0; i < v; i++) {
			glVertex2d(vert[i].getX(), vert[i].getY());
		}
		glEnd();
		delete[] vert;
	}
	Vec2D pos = getCameraAt(ex);
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex2d(pos.getX(), pos.getY());
	glEnd();
	glDisable(GL_SCISSOR_TEST);
	glPopMatrix();
}


void inline Level::handleChangedObjects() {
	//Safely handle anything that was added this update
	for (Entity* e : toAddE) {
		addEntity(e);
	}
	toAddE.clear();
	//Safely handle anything that was removed this update
	for (Entity* e : toRemoveE) {
		removeEntity(e);
	}
	toRemoveE.clear();
	for (Platform* p : toAddP) {
		addPlatform(p);
	}
	toAddE.clear();
	//Safely handle anything that was removed this update
	for (Platform* p : toRemoveP) {
		removePlatform(p);
	}
	toRemoveE.clear();
}


void inline Level::checkPlayerReachedGoal() {
	//Bespoke collision detection for goal (other checks need to be met)
	if (goalOpen && player && abs(player->getAngle() - goalAngle) < GOAL_ANGLE_DIF &&
		player->getPos().subtract(goal).magnitudeSquare() < GOAL_DISTANCE_SQR) {
		reachedGoal = true;
		removeEntity(player);
		player = NULL;
		//Give points for remaining time
		if (targetTime > levelTime - SPAWN_ANIM_END) {
			addScore(TIME_MULTIPLIER * (int)(targetTime - floor(levelTime - SPAWN_ANIM_END)));
		}
		alSourcePlay(goalSound);
	}
}


void inline Level::calculateBounds() {
	mapMinX = HUGE;
	mapMinY = HUGE;
	mapMaxX = -HUGE;
	mapMaxY = -HUGE;
	//Gets bounds of each platform
	//Doesn't need to be exact so forget about rotation
	//Just use the broadCheck distance
	for (Platform* p : platforms) {
		Vec2D pos = p->getPos();
		double d = p->getWidth() * p->getWidth() + p->getHeight() * p->getHeight();
		d = sqrt(d) / 2;
		if (pos.getX() - d < mapMinX) {
			mapMinX = pos.getX() - d;
		}
		if (pos.getX() + d > mapMaxX) {
			mapMaxX = pos.getX() + d;
		}
		if (pos.getX() - d < mapMinY) {
			mapMinY = pos.getY() - d;
		}
		if (pos.getY() + d > mapMaxY) {
			mapMaxY = pos.getY() + d;
		}
	}
	//Ditto for gravity fields
	for (GravityField* p : gravFields) {
		Vec2D pos = p->getPos();
		double d = p->getWidth() * p->getHeight();
		d = sqrt(d);
		if (pos.getX() - d < mapMinX) {
			mapMinX = pos.getX() - d;
		}
		if (pos.getX() + d > mapMaxX) {
			mapMaxX = pos.getX() + d;
		}
		if (pos.getX() - d < mapMinY) {
			mapMinY = pos.getY() - d;
		}
		if (pos.getY() + d > mapMaxY) {
			mapMaxY = pos.getY() + d;
		}
	}
}


// Calculates the force of gravity applied to an object at a location
void Level::getGravityAtPos(Vec2D pos, Vec2D* grav) {
	grav->set(0, 0);
	for (GravityField* f : gravFields) {
		//Translate point to be relative to the field's centre
		Vec2D p = pos.subtract(f->getPos());
		//Rotate the point back to be AA with the field (-angle)
		double cTheta = cos(-DEG_TO_RAD * f->getAngle()); //Slight optimisation as trig is expensive
		double sTheta = sin(-DEG_TO_RAD * f->getAngle());
		double xPrime = p.getX() * cTheta - p.getY() * sTheta;
		double yPrime = p.getY() * cTheta + p.getX() * sTheta;
		//Calculate field strength at point
		if (xPrime >= -f->getWidth() / 2 && xPrime <= f->getWidth() / 2
			&& yPrime >= -f->getHeight() / 2 && yPrime <= f->getHeight() / 2) {
			//Rotate this strength back and add it to force
			//(Forces have been rotated through 90 degrees to make 0 rotation = normal gravity
			grav->addTo(Vec2D(
				f->getStrength() * sin(DEG_TO_RAD * (180 - f->getAngle())),
				f->getStrength() * cos(DEG_TO_RAD * (180 - f->getAngle()))
			));
		}
	}
	if (grav->getX() == 0 && grav->getY() == 0) {
		grav->setY(-defaultGravity);
	}
}


// Gets the player entity
Player* Level::getPlayer() {
	return player;
}


// Gets the camera position ex seconds after last update
Vec2D Level::getCameraAt(double ex) {
	if (!player) {
		return reachedGoal ? goal : spawn;
	}
	//Extrapolate the position
	return player->getPos().add(player->getVel().multiply(ex));
}


// Gets the angle of the camera ex seconds after last update
double Level::getCameraAngleAt(double ex) {
	if (!player) {
		return spawnAngle;
	}
	//Get the angle of the player after it has updated
	return player->updatedVisAngle(ex);
}


// Loads the next level, provided it can be found
void Level::nextLevel() {
	//Check the level can actually be read
	DataObject check = DataObject();
	check.loadFromFile("Levels\\" + nextLevelPath);
	bool valid;
	check.getString("name", valid);
	if (!valid) {
		return;
	}
	loadLevel("Levels\\" + nextLevelPath);
}


// Loads the current level from the beginning
void Level::restartLevel() {
	//Check the level can actually be read
	DataObject check = DataObject();
	check.loadFromFile(levelPath);
	bool valid;
	check.getString("name", valid);
	if (!valid) {
		return;
	}
	loadLevel(levelPath);
}


// Sets whether the game is paused or not
void Level::setPause(bool p) {
	paused = p;
}


// Fails the level showing the fail screen
void Level::failLevel() {
	failed = true;
}


void Level::mouseEvent(GLFWwindow* window, int button, int action, int mods) {
	double dx, dy;
	glfwGetCursorPos(window, &dx, &dy);
	int x, y;
	x = (int)dx;
	y = sHeight - (int)dy;
	if (action == GLFW_RELEASE) {
		if (reachedGoal || failed) {
			buttonMenu.mouseDown(x, y);
			buttonRetry.mouseDown(x, y);
			if (reachedGoal) {
				buttonNext.mouseDown(x, y);
			}
		} else if (paused) {
			gradMenu.mouseDown(x, y);
			gradResume.mouseDown(x, y);
			gradRetry.mouseDown(x, y);
		}
	}
}


void Level::keyEvent(GLFWwindow* window, int key, int scan, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE && !reachedGoal && !failed) {
		paused = !paused;
	}
}


// Gets the current score
int Level::getScore() {
	return score;
}


// Sets the current score
void Level::setScore(int score) {
	this->score = score;
}


// Adds to the current score
void Level::addScore(int score) {
	this->score += score;
}


// Safely add an entity during an update call
void Level::safeAdd(Entity* e) {
	toAddE.push_back(e);
}


// Safely remove an entity during an update call
void Level::safeDelete(Entity* e) {
	toRemoveE.push_back(e);
}


// Safely add a platform during an update call
void Level::safeAdd(Platform* p) {
	toAddP.push_back(p);
}


// Safely remove a platform during an update call
void Level::safeDelete(Platform* p) {
	toRemoveP.push_back(p);
}


// Sets a channel to be active next update
void Level::setChannel(int c) {
	if (c < 0 || c >= MAX_CHANNELS) {
		return;
	}
	int byte = c / 8;
	char bits = 0x1 << (c%8);
	newChannels[byte] = newChannels[byte] | bits;
}


// Gets whether a channel is active
bool Level::getChannel(int c) {
	//Negative channels represent TRUE
	if (c < 0) {
		return true;
	}
	if (c >= MAX_CHANNELS) {
		return false;
	}
	int byte = c / 8;
	char bits = 0x1 << (c % 8);
	return (channels[byte] & bits)!=0;
}
