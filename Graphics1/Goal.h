#pragma once
#include "Platform.h"
class Goal:
	public Platform {
public:
	Goal();
	~Goal();
	bool canResize();
	bool canDelete();
	bool onMove(double dx, double dy);
	bool onRotate(double da);
	void draw(double ex);
};

