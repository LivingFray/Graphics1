#pragma once
#include "Entity.h"
#include "ParticleSystem.h"
class ShieldGiver :
	public Entity {
public:
	ShieldGiver();
	~ShieldGiver();
	// Updates the entity
	void update();
	// Called when damage is inflicted on the object
	void onDamage(Damage d);
	// Draws the player, ex seconds from last update
	void draw(double ex);
	// Called when a collision occurs
	void onCollide(Collider* other);
	// Returns whether the object is solid
	bool isSolid();
protected:
	Entity* following;
	double deathTime;
	double lastTime;
	bool dead;
	ParticleSystem sparks;
	Animation anim;
	ALuint sparkSound;
};

