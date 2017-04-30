#pragma once
#include "Entity.h"
class Interactive :
	public Entity {
public:
	Interactive();
	virtual ~Interactive();
protected:
	bool getNear();
};

