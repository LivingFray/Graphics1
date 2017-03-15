#pragma once
#include "Entity.h"
class PointGiver:
	public Entity {
public:
	PointGiver();
	~PointGiver();
	void onCollide(Collider* other);
	// Updates the entity
	void update();
};

