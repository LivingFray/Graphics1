#pragma once
#include "Entity.h"
class Projectile :
	public Entity {
public:
	Projectile();
	~Projectile();
	// Called when a collision occurs
	void onCollide(Collider* other);
	// Sets the owner of the projectile
	void setOwner(Entity* e);
protected:
	Animation anim;
	Entity* owner;
	ALuint fireSound;
};

