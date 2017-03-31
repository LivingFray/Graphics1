#include "SpawnPoint.h"
#include "LevelEditor.h"


SpawnPoint::SpawnPoint() {
	id = "spawn";
	pos = Vec2D(0.0, 0.0);
	//Make the spawn large enough to hold the player
	width = SPAWN_WIDTH;
	height = SPAWN_HEIGHT;
	angle = 0.0;
}


SpawnPoint::~SpawnPoint() {
}


// Returns if the selectable can be resized
bool SpawnPoint::canResize() {
	return false;
}


// Returns if the selectable can be deleted
bool SpawnPoint::canDelete() {
	return false;
}


// Called when the selectable is moved
bool SpawnPoint::onMove(double dx, double dy) {
	LevelEditor* editor = (LevelEditor*)state;
	if (Platform::onMove(dx, dy)) {
		editor->setSpawn(pos);
		return true;
	}
	return false;
}


// Called when the selectable is rotated
bool SpawnPoint::onRotate(double da) {
	LevelEditor* editor = (LevelEditor*)state;
	if (Platform::onRotate(da)) {
		editor->setSpawnAngle(angle);
		return true;
	}
	return false;
}


// Draws the spawnpoint ex seconds after last update
void SpawnPoint::draw(double ex) {
}


// Sets the options for this selectable
void SpawnPoint::setOptions(OptionMenu* menu) {
	Selectable::setOptions(menu);
	LevelEditor* editor = (LevelEditor*)state;
	editor->setSpawnAngle(angle);
	editor->setSpawn(pos);
}


// Creates an option menu using the current values as defaults
void SpawnPoint::createOptions() {
	Selectable::createOptions();
}


// Returns a DataObject representing the storable object
DataObject SpawnPoint::save() {
	DataObject platform = Storable::save();
	return platform;
}


// Loads the storable object from the DataObject
void SpawnPoint::load(DataObject obj) {
	Storable::load(obj);
}
