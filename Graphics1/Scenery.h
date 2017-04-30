#pragma once
#include "Animation.h"
#include "Selectable.h"
#include "Storable.h"
class Scenery :
	public Selectable,
	public Storable {
public:
	Scenery();
	virtual ~Scenery();
	// Updates the scenery
	virtual void update();
	// Draws the scenery
	virtual void draw(double ex);
protected:
	Animation* currentAnim;
};

