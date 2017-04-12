#pragma once
#include "Entity.h"

class Player:
	public Entity {
public:
	Player();
	~Player();
	// Updates the player
	void update();
	// Draws the player, ex seconds from last update
	void draw(double ex);
	// Called when damage is inflicted on the object
	void onDamage(Damage d);
};

