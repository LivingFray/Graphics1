#include "Goal.h"
#include "LevelEditor.h"


Goal::Goal() {
	id = "goal";
	pos = Vec2D(0.0, 0.0);
	//Make the spawn large enough to hold the player
	width = SPAWN_WIDTH;
	height = SPAWN_HEIGHT;
	angle = 0.0;
}


Goal::~Goal() {
}


// Returns if the selectable can be resized
bool Goal::canResize() {
	return false;
}


// Returns if the selectable can be deleted
bool Goal::canDelete() {
	return false;
}


// Returns if the selectable can by copied
bool Goal::canCopy() {
	return false;
}


// Called when the selectable is moved
bool Goal::onMove(double dx, double dy) {
	LevelEditor* editor = (LevelEditor*)state;
	if (Platform::onMove(dx, dy)) {
		editor->setGoal(pos);
		return true;
	}
	return false;
}


// Called when the selectable is rotated
bool Goal::onRotate(double da) {
	LevelEditor* editor = (LevelEditor*)state;
	if (Platform::onRotate(da)) {
		editor->setGoalAngle(angle);
		return true;
	}
	return false;
}


// Draws the goal ex seconds after last update
void Goal::draw(double ex) {
}


// Sets the options for this selectable
void Goal::setOptions(OptionMenu* menu) {
	Selectable::setOptions(menu);
	LevelEditor* editor = (LevelEditor*)state;
	editor->setGoalAngle(angle);
	editor->setGoal(pos);
}


// Creates an option menu using the current values as defaults
void Goal::createOptions() {
	Selectable::createOptions();
}


// Returns a DataObject representing the storable object
DataObject Goal::save() {
	DataObject platform = Storable::save();
	return platform;
}


// Loads the storable object from the DataObject
void Goal::load(DataObject obj) {
	Storable::load(obj);
}
