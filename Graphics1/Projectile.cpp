#include "Projectile.h"
#include "Level.h"


Projectile::Projectile() {
	id = "projectile";
	width = 0.25;
	height = 0.25;
	anim = Animation();
	anim.setSpriteSheet("Resources\\entities\\projectile.png");
	anim.setSpritesheetSize(1, 1);
	anim.setSize(0.25, 0.25);
	anim.addFrame(0, 1);
	anim.setRepeat(true);
	currentAnim = &anim;
	//Still affected by gravity, but not as strongly
	grav = 0.05;
	fireSound = SoundLoader::getSound("Resources\\sounds\\fire.wav");
	alSourcePlay(fireSound);
}


Projectile::~Projectile() {
	alDeleteSources(1, &fireSound);
}


// Called when a collision occurs
void Projectile::onCollide(Collider* other) {
	//Don't collide with the entity that created you
	if (other == owner) {
		return;
	}
	other->onDamage(Damage::PROJECTILE);
	Level* l = (Level*)state;
	l->safeDelete(this);
}


// Sets the owner of the projectile
void Projectile::setOwner(Entity* e) {
	owner = e;
}
