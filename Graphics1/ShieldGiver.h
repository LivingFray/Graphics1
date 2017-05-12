#pragma once
#include "Entity.h"
#include "ParticleSystem.h"
class ShieldGiver :
	public Entity {
public:
	ShieldGiver();
	ShieldGiver(const ShieldGiver & other);
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
	// Sets the desired position to occupy relative to the player
	void setDesiredPos(Vec2D pos);
protected:
	Entity* following;
	double deathTime;
	double lastTime;
	bool dead;
	ParticleSystem sparks;
	Animation anim;
	ALuint sparkSound;
	Vec2D desiredPos;
};

