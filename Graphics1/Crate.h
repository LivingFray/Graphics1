#pragma once
#include "Entity.h"
class Crate :
	public Entity {
public:
	Crate();
	Crate(const Crate& other);
	~Crate();
protected:
	Animation anim;
};

