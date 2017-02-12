#include "Level.h"
#include "Player.h"
#include "KeyConfig.h"
#include "Globals.h"
#include <GLFW\glfw3.h>

//How much the player slows down in the horizontal axis when not moving
#define PLAYER_FRICTION 2
//How fast the player accelerates when pressing a direction key
#define PLAYER_ACCELERATION 30
//How fast the player jumps
#define PLAYER_JUMP 80

Player::Player() {
}


Player::~Player() {
}

// Updates the player
void Player::update() {
	//Get direction of gravity
	Vec2D g;
	level->getGravityAtPos(pos, &g);
	//The velocity in the frame of the current gravity
	double vX = getVelRelX(angle);
	double dX = 0.0;
	if (KeyConfig::isDown("moveLeft")) {
		dX -= PLAYER_ACCELERATION;
	}
	if (KeyConfig::isDown("moveRight")) {
		dX += PLAYER_ACCELERATION;
	}
	//Slow down if not trying to move
	if (abs(dX) <= FLOAT_ZERO) {
		if (vX > PLAYER_FRICTION) {
			dX -= PLAYER_FRICTION;
		}
		else if (vX < -PLAYER_FRICTION) {
			dX += PLAYER_FRICTION;
		}
		else {
			dX = -vX;
		}
	}
	//Rotate the vector through 90 degrees
	Vec2D move = Vec2D(-g.getY(), g.getX());
	//Normalise the value
	if (move.magnitudeSquare() > FLOAT_ZERO) {
		move.toUnit();
	}
	else {
		move.set(1.0, 0.0);
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
		}
		else {
			jump.set(0.0,-1.0); //Just pretend that gravity is normal
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
