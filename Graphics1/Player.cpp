#include "Player.h"
#include "KeyConfig.h"
#include "Globals.h"
#include <GLFW\glfw3.h>

//How much the player slows down in the horizontal axis when not moving
#define PLAYER_FRICTION 20
//How fast the player accelerates when pressing a direction key
#define PLAYER_ACCELERATION 80
//How fast the player jumps
#define PLAYER_JUMP 80

Player::Player() {
}


Player::~Player() {
}

// Updates the player
void Player::update() {
	//Get direction of gravity
	double gX;
	double gY;
	level->getGravityAtPos(posX, posY, &gX, &gY);
	//TODO: smoothing rotation?
	if (abs(gX) < FLOAT_ZERO && abs(gY) < FLOAT_ZERO) {
		angle = 0; //Floating in no gravity
	}
	else {
		angle = atan2(gY, gX) * RAD_TO_DEG + 90;
	}
	//The velocity in the frame of the current gravity
	double vX = getVelRelX(angle);
	double dX = 0.0;
	if (keyconfig::isDown("moveLeft")) {
		dX -= PLAYER_ACCELERATION;
	}
	if (keyconfig::isDown("moveRight")) {
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
	glfwSetWindowTitle(keyconfig::win, (to_string(posX) + ", " + to_string(posY)).c_str());
	//Apply gravity to velocity
	velX += gX;
	velY += gY;
	//Rotate the vector through 90 degrees
	double moveX = -gY;
	double moveY = gX;
	//Normalise the value
	double mag = sqrt(moveX * moveX + moveY * moveY);
	//We appear to be floating in an area of 0 gravity
	if (mag == 0) {
		moveX = 1; //Just pretend that gravity is normal
	}
	else {
		moveX /= mag;
		moveY /= mag;
	}
	//Scale it to the movement
	moveX *= dX;
	moveY *= dX;
	velX += moveX;
	velY += moveY;
	//TODO: jumping here
	if (keyconfig::isDown("jump")) {
		double jumpX = gX;
		double jumpY = gY;
		//Normalise vector
		//We appear to be floating in an area of 0 gravity
		if (mag == 0) {
			jumpY = -1; //Just pretend that gravity is normal
		}
		else {
			jumpX /= mag;
			jumpY /= mag;
		}
		jumpX *= PLAYER_JUMP;
		jumpY *= PLAYER_JUMP;
		velX -= jumpX;
		velY -= jumpY;
	}
	//Call inherited update (handles moving)
	Entity::update();
}


// Draws the player, ex seconds from last update
void Player::draw(double ex) {
	Entity::draw();
}
