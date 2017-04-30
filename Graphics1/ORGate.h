#pragma once
#include "LogicGate.h"
class ORGate :
	public LogicGate {
public:
	ORGate();
	~ORGate();
	// Updates the logic gate
	void update();
};

