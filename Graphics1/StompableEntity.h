#pragma once
#include "EntityAI.h"
class StompableEntity :
	public EntityAI {
public:
	StompableEntity();
	~StompableEntity();
	// Called when damage is inflicted on the object
	void onDamage(Damage d);
	// Called when a collision occurs
	void onCollide(Collider * other);
protected:
	Animation idleAnim;
};

