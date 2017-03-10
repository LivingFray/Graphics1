#pragma once
#include "Platform.h"
class SpawnPoint:
	public Platform {
public:
	SpawnPoint();
	~SpawnPoint();
	bool canResize();
	bool canDelete();
	bool onMove(double dx, double dy);
	bool onRotate(double da);
	void draw(double ex);
};

