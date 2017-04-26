#pragma once
#include "StompableEntity.h"
class Turret :
	public StompableEntity {
public:
	Turret();
	~Turret();
	// Updates the entity
	void update();
protected:
	double cooldown;
};

