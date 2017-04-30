#pragma once
#include "LogicGate.h"
class XNORGate :
	public LogicGate {
public:
	XNORGate();
	~XNORGate();
	// Updates the logic gate
	void update();
};

