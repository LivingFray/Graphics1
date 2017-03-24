#include "Level.h"
#include "Player.h"
#include "Globals.h"

Player::Player() {
	id = "player";
	width = PLAYER_WIDTH;
	height = PLAYER_HEIGHT;
	flip = false;
	idle = ImageLoader::getImage("Resources\\playerIdle.png");
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
	//Slow down if not trying to move
	if (abs(dX) <= FLOAT_ZERO) {
		double fr = PLAYER_FRICTION;
		if (onGround) {
			fr = PLAYER_FRICTION * 5;
		}
		if (vX > fr) {
			dX -= fr;
		} else if (vX < -fr) {
			dX += fr;
		} else {
			dX = -vX;
		}
	}
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
