#pragma once
#include "LevelRenderer.h"
#include "BaseState.h"
#include "GradButton.h"
//The number of buttons in the editor bar
#define EDITOR_BAR_BUTTONS 5
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
	void mouseMoveEvent(GLFWwindow* window, double x, double y);
	// Sets whether the item menu is visible
	void setInItemMenu(bool inMenu);
	// Gets the current position of the camera
	Vec2D getCameraPos();
	// Gets the position of the camera ex seconds after last update
	Vec2D getCameraAt(double ex);
	// Gets the angle of the camera ex seconds after last update
	double getCameraAngleAt(double ex);
	// Gets the file location to save the level to
	string getFileLocation();
	// Saves the level to the given file
	void saveLevel(string filePath);
	// Loads a level from the given file
	void loadLevel(string filePath);
protected:
	bool inItemMenu;
	bool inSaveMenu;
	struct MenuItem {
		string name;
		void(*create)(BaseState* l);
	};
	vector<MenuItem> menuItems;
	vector<GradButton*> buttons;
	Vec2D camPos;
	double camAngle;
	Vec2D panFrom;
	bool panning;
	// Updates the camera position and rotation
	void updateCamera(double time);
	int current;
	GLuint barButtons[EDITOR_BAR_BUTTONS];
	Selectable* selected;
	Vec2D moveDir;
	bool moving;
	Vec2D scaleDir;
	int resizing;
	double rotateFrom;
	bool rotating;
	GLuint rotateCall;
	void select(Vec2D world);
	Button saveButton;
	Button loadButton;
	string fileLoc;
};

