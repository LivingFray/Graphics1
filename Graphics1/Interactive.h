#pragma once
#include "Scenery.h"
class Interactive :
	public Scenery {
public:
	Interactive();
	virtual ~Interactive();
protected:
	bool getNear();
};

