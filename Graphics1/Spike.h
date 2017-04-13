#pragma once
#include "Platform.h"
class Spike :
	public Platform {
public:
	Spike();
	~Spike();
	// Called when a collision occurs
	void onCollide(Collider* other);
};

