#include "SpawnPoint.h"
#include "LevelEditor.h"


SpawnPoint::SpawnPoint() {
	id = "spawn";
	pos = Vec2D(0.0, 0.0);
	//Make the spawn large enough to hold the player
	width = PLAYER_WIDTH * SPAWN_SCALE;
	height = PLAYER_HEIGHT * SPAWN_SCALE;
	angle = 0.0;
}


SpawnPoint::~SpawnPoint() {
}


bool SpawnPoint::canResize() {
	return false;
}

bool SpawnPoint::canDelete() {
	return false;
}

bool SpawnPoint::onMove(double dx, double dy) {
	LevelEditor* editor = (LevelEditor*)state;
	if (Platform::onMove(dx, dy)) {
		editor->setSpawn(pos);
		return true;
	}
	return false;
}


bool SpawnPoint::onRotate(double da) {
	LevelEditor* editor = (LevelEditor*)state;
	if (Platform::onRotate(da)) {
		editor->setSpawnAngle(angle);
		return true;
	}
	return false;
}

void SpawnPoint::draw(double ex) {

}