#pragma once
#include "EntityAI.h"
class BombEntity :
	public EntityAI {
public:
	BombEntity();
	~BombEntity();
	//Updates the entity
	void update();
	// Called when damage is inflicted on the object
	void onDamage(Damage d);
protected:
	double fuse;
	bool triggered;
	Animation explodeAnim;
	Animation idleAnim;
	void explode();
	ALuint tickSound;
	double tickTime;
	double maxTick;
};

