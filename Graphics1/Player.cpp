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
}


Player::~Player() {
}


// Updates the player
void Player::update() {
	//Get direction of gravity
	Vec2D g;
	Level* level = (Level*)state;
	level->getGravityAtPos(pos, &g);
	//The velocity in the frame of the current gravity
	double vX = getVelRelX(angle);
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
	//Rotate the vector through 90 degrees
	Vec2D move = Vec2D(-g.getY(), g.getX());
	//Normalise the value
	if (move.magnitudeSquare() > FLOAT_ZERO) {
		move.toUnit();
	} else {
		move.set(cos(DEG_TO_RAD * angle), sin(DEG_TO_RAD * angle));
	}
	//Scale it to the movement
	move.multiplyBy(dX);
	//Add the movement
	vel.addTo(move);
	//Handle jumping
	if (KeyConfig::isDown("jump") && onGround) {
		Vec2D jump = g;
		//Normalise vector
		//We appear to be floating in an area of 0 gravity
		if (jump.magnitudeSquare() > FLOAT_ZERO) {
			jump.toUnit();
		} else {
			jump.set(0.0, -1.0); //Just pretend that gravity is normal
		}
		jump.multiplyBy(PLAYER_JUMP);
		vel.subtractFrom(jump);
	}
	//Call inherited update (handles moving)
	Entity::update();
}


// Draws the player, ex seconds from last update
void Player::draw(double ex) {
	Entity::draw(ex);
}
