#pragma once
#include "Entity.h"
class EntityAI :
	public Entity {
public:
	EntityAI();
	~EntityAI();
	// Updates the entity
	virtual void update();
};

