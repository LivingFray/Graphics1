#pragma once
#include "LogicGate.h"
class NANDGate :
	public LogicGate {
public:
	NANDGate();
	~NANDGate();
	// Updates the logic gate
	void update();
};

