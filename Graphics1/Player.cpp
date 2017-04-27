#include "Level.h"
#include "Player.h"
#include "Globals.h"

Player::Player() {
	id = "player";
	width = PLAYER_WIDTH;
	height = PLAYER_HEIGHT;
	flip = false;
	walkAnim = Animation();
	walkAnim.setPos(Vec2D(0.0, 0.0));
	walkAnim.setRepeat(true);
	walkAnim.setSize(1.0, 2.0);
	walkAnim.setSpriteSheet("Resources\\entities\\playerWalk.png");
	for (int i = 0; i < 8; i++) {
		walkAnim.addFrame(i, (36/(32.0*DEFAULT_ENTITY_MAXSPEED*8)));
	}
	walkAnim.setSpritesheetSize(4, 2);
	idleAnim = Animation();
	idleAnim.setPos(Vec2D(0.0, 0.0));
	idleAnim.setRepeat(false);
	idleAnim.setSize(1.0, 2.0);
	idleAnim.setSpriteSheet("Resources\\entities\\playerIdle.png");
	idleAnim.addFrame(0, 1);
	idleAnim.setSpritesheetSize(1, 1);
	jumpAnim = Animation();
	jumpAnim.setPos(Vec2D(0.0, 0.0));
	jumpAnim.setRepeat(false);
	jumpAnim.setSize(1.0, 2.0);
	jumpAnim.setSpriteSheet("Resources\\entities\\playerJump.png");
	jumpAnim.addFrame(0, 1);
	jumpAnim.setSpritesheetSize(1, 1);
	shield = ImageLoader::getImage("Resources\\entities\\shield.png");
	immuneTime = 0;
	currentAnim = &idleAnim;
	wasMoving = false;
	offCount = 0;
}


Player::~Player() {
}


// Updates the player
void Player::update() {
	double dX = 0.0;
	if (KeyConfig::isDown("moveLeft")) {
		dX -= PLAYER_ACCELERATION;
		if (!flip) {
			updateShields();
		}
		flip = true;
	}
	if (KeyConfig::isDown("moveRight")) {
		dX += PLAYER_ACCELERATION;
		if (flip) {
			updateShields();
		}
		flip = false;
	}
	//Set whether the player is moving
	moving = abs(dX) > FLOAT_ZERO;
	if (moving) {
		currentAnim = &walkAnim;
		if (!wasMoving) {
			currentAnim->setTime(0);
			wasMoving = true;
		}
	} else {
		wasMoving = false;
		currentAnim = &idleAnim;
		currentAnim->setTime(0);
	}
	if (!onGround) {
		offCount++;
		if (offCount > 5) {
			currentAnim = &jumpAnim;
			offCount = 5; //Prevent overflows
		}
	} else {
		offCount = 0;
	}
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
	//Draw any shields the player may have
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
		shields.front()->onDamage(Damage::INSTAKILL);
		shields.pop_front();
		immuneTime = 1;
		updateShields();
	} else {
		Level* l = (Level*)state;
		l->failLevel();
	}
}


// Gives the player a shield granting immunity to 1 attack
void Player::giveShield(ShieldGiver* giver) {
	shields.push_back(giver);
	giver->setDesiredPos(Vec2D(shields.size(), 1.5));
}


//Updates the desired locations of the shields
void Player::updateShields() {
	int i = 0;
	for (ShieldGiver* s : shields) {
		s->setDesiredPos(Vec2D(i * (flip?-1:1),1.5));
		i++;
	}
}