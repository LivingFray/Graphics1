#pragma once
#include "Interactive.h"
class Lever :
	public Interactive {
public:
	Lever();
	~Lever();
	// Updates the entity
	void update();
protected:
	Animation sOffAnim;
	Animation nOffAnim;
	Animation sOnAnim;
	Animation nOnAnim;
	bool selected;
	bool active;
	bool wasDown;
	int channel;
};

