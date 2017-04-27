#pragma once
#include "Platform.h"
class BreakablePlatform :
	public Platform {
public:
	BreakablePlatform();
	~BreakablePlatform();
	// Called when damage is inflicted on the object
	void onDamage(Damage d);
};

