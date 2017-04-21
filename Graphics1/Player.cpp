#include "Level.h"
#include "Player.h"
#include "Globals.h"

Player::Player() {
	id = "player";
	width = PLAYER_WIDTH;
	height = PLAYER_HEIGHT;
	flip = false;
	//idle = ImageLoader::getImage("Resources\\playerIdle.png");
	//TODO: Different frames for different actions
	currentAnim = Animation();
	currentAnim.setPos(Vec2D(0.0, 0.0));
	currentAnim.setRepeat(true);
	currentAnim.setSize(1.0, 2.0);
	currentAnim.addFrame(0, 1.0);
	currentAnim.addFrame(1, 0.5);
	currentAnim.setSpriteSheet("Resources\\playerSprite.png");
	//TEMP, make correct size when I have textures
	currentAnim.setSpritesheetSize(2, 2);
	shield = ImageLoader::getImage("Resources\\entities\\shield.png");
	immuneTime = 0;
}


Player::~Player() {
}


// Updates the player
void Player::update() {
	double dX = 0.0;
	if (KeyConfig::isDown("moveLeft")) {
		dX -= PLAYER_ACCELERATION;
		flip = true;
	}
	if (KeyConfig::isDown("moveRight")) {
		dX += PLAYER_ACCELERATION;
		flip = false;
	}
	//Set whether the player is moving
	moving = abs(dX) > FLOAT_ZERO;
	//Set direction of movement
	Vec2D move = Vec2D(cos(DEG_TO_RAD * visAngle), sin(DEG_TO_RAD * visAngle));
	//Scale it to the movement
	move.multiplyBy(dX);
	//Add the movement
	vel.addTo(move);
	//Handle jumping
	if (KeyConfig::isDown("jump") && onGround) {
		Vec2D jump;
		//Get direction of gravity
		Level* level = (Level*)state;
		level->getGravityAtPos(pos, &jump);
		//Normalise vector
		//We appear to be floating in an area of 0 gravity
		if (jump.magnitudeSquare() > FLOAT_ZERO) {
			jump.toUnit();
		} else {
			jump.set(cos(DEG_TO_RAD * angle), sin(DEG_TO_RAD * angle)); //Use current angle
		}
		jump.multiplyBy(PLAYER_JUMP);
		vel.subtractFrom(jump);
	}
	//Handle damage immunity timer
	if (immuneTime > 0) {
		immuneTime -= TICKRATE;
		if (immuneTime < 0) {
			immuneTime = 0;
		}
	}
	//Call inherited update (handles moving)
	Entity::update();
}


// Draws the player, ex seconds from last update
void Player::draw(double ex) {
	Entity::draw(ex);
	if (shields.size() > 0 || immuneTime > 0) {
		Vec2D p = pos.add(vel.multiply(ex));
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, shield);
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glTranslated(p.getX(), p.getY(), 0.0);
		glRotated(visAngle, 0.0, 0.0, 1.0);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0);
		glVertex2d(-0.5, -1.0);
		glTexCoord2d(0.0, 1.0);
		glVertex2d(-0.5, 1.0);
		glTexCoord2d(1.0, 1.0);
		glVertex2d(0.5, 1.0);
		glTexCoord2d(1.0, 0.0);
		glVertex2d(0.5, -1.0);
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
}


// Called when damage is inflicted on the object
void Player::onDamage(Damage d) {
	if (immuneTime > 0) {
		return;
	}
	if (shields.size() > 0) {
		shields.back()->onDamage(Damage::INSTAKILL);
		shields.pop_back();
		immuneTime = 1;
	} else {
		Level* l = (Level*)state;
		l->failLevel();
	}
}


// Gives the player a shield granting immunity to 1 attack
void Player::giveShield(Entity* giver) {
	shields.push_back(giver);
}
