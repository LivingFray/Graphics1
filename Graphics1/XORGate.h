#pragma once
#include "LogicGate.h"
class XORGate :
	public LogicGate {
public:
	XORGate();
	~XORGate();
	// Updates the logic gate
	void update();
};

