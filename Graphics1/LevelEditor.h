#pragma once
#include "LevelRenderer.h"
#include "BaseState.h"
#include "GradButton.h"
#include "TextBox.h"
//The number of buttons in the editor bar
#define EDITOR_BAR_BUTTONS 6
class LevelEditor:
	public BaseState, 
	public LevelRenderer {
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
	void textEvent(GLFWwindow*, unsigned int ch);
	void resizeEvent(GLFWwindow* window, int width, int height);
	// Sets the current menu
	void setMenu(int m);
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
	// Adds a platform to the level
	void addPlatform(Platform* platform);
	// Adds a gravity field to the level
	void addGravityField(GravityField* field);
private:
	enum class Menu {NONE, ITEM, SAVE, OPTIONS};
	Menu currentMenu;
	struct MenuItem {
		string name;
		void(*create)(BaseState* l);
	};
	vector<MenuItem> menuItems;
	vector<Button*> buttons;
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
	TextBox fileBox;
	TextBox levelBox;
	TextBox nextBox;
	TextBox gravBox;
	TextBox targetBox;
	GradButton exitButton;
	GradButton returnButton;
	OptionMenu* optMenu;
	void drawTextBox(string label, TextBox &box, int y);
	void drawSmallTextBox(string label, TextBox & box, bool left, int y);
	void drawHandle(double x, double y);
};

