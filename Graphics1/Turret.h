#pragma once
#include "StompableEntity.h"
class Turret :
	public StompableEntity {
public:
	Turret();
	Turret(const Turret& other);
	~Turret();
	// Updates the entity
	void update();
protected:
	double cooldown;
};

