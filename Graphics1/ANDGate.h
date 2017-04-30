#pragma once
#include "LogicGate.h"
class ANDGate :
	public LogicGate {
public:
	ANDGate();
	~ANDGate();
	// Updates the logic gate
	void update();
};

