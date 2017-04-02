#pragma once
#include "EntityAI.h"
class BombEntity :
	public EntityAI {
public:
	BombEntity();
	~BombEntity();
	//Updates the entity
	void update();
};

