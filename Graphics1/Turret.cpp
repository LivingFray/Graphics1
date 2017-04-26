#include "Turret.h"
#include "Level.h"
#include "Projectile.h"


Turret::Turret() {
	id = "turret";
	maxSpeed = 0;
	cooldown = SHOOTING_COOLDOWN;
	width = 0.5;
	height = 1.0;
	idleAnim.setSpriteSheet("Resources\\entities\\turretAlive.png");
	idleAnim.setSize(0.5, 1.0);
	deadAnim.setSpriteSheet("Resources\\entities\\turretDead.png");
	deadAnim.setSize(0.5, 1.0);
}


Turret::~Turret() {
}


// Updates the entity
void Turret::update() {
	if (!dead) {
		if (cooldown > 0) {
			cooldown -= TICKRATE;
		}
		Level* l = (Level*)state;
		Player* p = l->getPlayer();
		if (!p) {
			return;
		}
		double d = p->getPos().subtract(pos).magnitudeSquare();
		if (d < SHOOTING_VISION * SHOOTING_VISION) {
			//Calculate desired direction to travel
			Vec2D dis = p->getPos().subtract(pos);
			//Get direction possible to look in
			Vec2D grav;
			l->getGravityAtPos(pos, &grav);
			//If angle between grav and move is <180 look right, else left
			//If move x grav < 0 => right
			double det = dis.getX()*grav.getY() - grav.getX()*dis.getY();
			//Face correct direction
			if (det < 0) {
				flip = false;
			} else {
				flip = true;
			}
			if (cooldown <= 0) {
				cooldown = SHOOTING_COOLDOWN;
				//Make projectile here
				Projectile* proj = new Projectile();
				dis.toUnit();
				proj->setPos(pos);
				proj->setVel(dis.multiply(PROJECTILE_SPEED));
				proj->setOwner(this);
				l->safeAdd(proj);
			}
		}
	}
	//Just handle moving/falling, no AI
	moving = false;
	Entity::update();
}