#include "Goal.h"
#include "LevelEditor.h"


Goal::Goal() {
	id = "goal";
	pos = Vec2D(0.0, 0.0);
	//Make the spawn large enough to hold the player
	width = PLAYER_WIDTH * SPAWN_SCALE;
	height = PLAYER_HEIGHT * SPAWN_SCALE;
	angle = 0.0;
}


Goal::~Goal() {
}


bool Goal::canResize() {
	return false;
}

bool Goal::canDelete() {
	return false;
}

bool Goal::onMove(double dx, double dy) {
	LevelEditor* editor = (LevelEditor*)state;
	if (Platform::onMove(dx, dy)) {
		editor->setGoal(pos);
		return true;
	}
	return false;
}


bool Goal::onRotate(double da) {
	LevelEditor* editor = (LevelEditor*)state;
	if (Platform::onRotate(da)) {
		editor->setGoalAngle(angle);
		return true;
	}
	return false;
}

void Goal::draw(double ex) {

}