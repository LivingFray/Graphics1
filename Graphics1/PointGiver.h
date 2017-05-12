#pragma once
#include "Entity.h"
class PointGiver:
	public Entity {
public:
	PointGiver();
	PointGiver(const PointGiver& other);
	~PointGiver();
	void onCollide(Collider* other);
	// Updates the entity
	void update();
protected:
	Animation anim;
};

