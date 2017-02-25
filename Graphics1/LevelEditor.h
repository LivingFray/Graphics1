#pragma once
#include "LevelRenderer.h"
#include "BaseState.h"
#include "GradButton.h"
class LevelEditor:
	public BaseState, LevelRenderer {
public:
	LevelEditor();
	~LevelEditor();
	// Updates the editor
	void update();
	// Draws the level
	void draw(double ex);
	void keyEvent(GLFWwindow* window, int key, int scan, int action, int mods);
	void mouseEvent(GLFWwindow* window, int button, int action, int mods);
	// Sets whether the item menu is visible
	void setInItemMenu(bool inMenu);
	// Gets the current position of the camera
	Vec2D getCameraPos();
	// Gets the angle of the camera ex seconds after last update
	virtual double getCameraAngleAt(double ex);
protected:
	bool inItemMenu;
	struct MenuItem {
		string name;
		void(*create)(BaseState* l);
	};
	vector<MenuItem> menuItems;
	vector<GradButton*> buttons;
	Vec2D camPos;
	double camAngle;
};

