#pragma once
#include "LogicGate.h"
class NORGate :
	public LogicGate {
public:
	NORGate();
	~NORGate();
	// Updates the logic gate
	void update();
};

