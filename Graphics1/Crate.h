#pragma once
#include "Entity.h"
class Crate :
	public Entity {
public:
	Crate();
	~Crate();
protected:
	Animation anim;
};

