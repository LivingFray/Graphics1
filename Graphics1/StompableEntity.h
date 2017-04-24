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
	// Updates the entity
	void update();
protected:
	Animation idleAnim;
	Animation deadAnim;
	double deadTime;
};

