#include "Turret.h"
#include "Level.h"
#include "Projectile.h"


Turret::Turret() {
	id = "turret";
	maxSpeed = 0;
	cooldown = SHOOTING_COOLDOWN;
	maxCooldown = SHOOTING_COOLDOWN;
	width = 0.5;
	height = 1.0;
	idleAnim.setSpriteSheet("Resources\\entities\\turretAlive.png");
	idleAnim.setSize(0.5, 1.0);
	deadAnim.setSpriteSheet("Resources\\entities\\turretDead.png");
	deadAnim.setSize(0.5, 1.0);
	maxRange = SHOOTING_VISION;
}


Turret::Turret(const Turret& other) : WorldObject(other), StompableEntity(other) {
	cooldown = other.cooldown;
	idleAnim = other.idleAnim;
	deadAnim = other.deadAnim;
	maxRange = other.maxRange;
	maxCooldown = other.maxCooldown;
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
		if (d < maxRange * maxRange) {
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
				cooldown = maxCooldown;
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


// Sets the options for this selectable
void Turret::setOptions(OptionMenu* menu) {
	Entity::setOptions(menu);
	std::map<string, string> values = menu->getValues();
	string v;
	v = values["Activation Range"];
	maxRange = atof(v.c_str());
	v = values["Cooldown"];
	maxCooldown = atof(v.c_str());
}


// Creates an option menu using the current values as defaults
void Turret::createOptions() {
	Entity::createOptions();
	options->addOption("Activation Range", true, to_string(maxRange));
	options->addOption("Cooldown", true, to_string(maxCooldown));
}


// Returns a DataObject representing the storable object
DataObject Turret::save() {
	DataObject sc = Entity::save();
	sc.add("maxRange", maxRange);
	sc.add("cooldown", maxCooldown);
	return sc;
}


// Loads the storable object from the DataObject
void Turret::load(DataObject obj) {
	Entity::load(obj);
	maxRange = obj.getDouble("maxRange");
	maxCooldown = obj.getDouble("cooldown");
}
