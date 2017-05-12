#pragma once
#include "LevelBase.h"
#include "BaseState.h"
#include "GradButton.h"
#include "TextBox.h"
//The number of buttons in the editor bar
#define EDITOR_BAR_BUTTONS 7
class LevelEditor:
	public BaseState, 
	public LevelBase {
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
	Vec2D dragFrom;
	bool dragging;
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
	TextBox goalBox;
	TextBox camXBox;
	TextBox camYBox;
	TextBox camABox;
	GradButton exitButton;
	GradButton returnButton;
	OptionMenu* optMenu;
	vector<TextBox*> textBoxes;
	void drawTextBox(string label, TextBox &box, int y);
	void drawSmallTextBox(string label, TextBox & box, bool left, int y);
	void drawHandle(double x, double y);
	void drawMovingHandles(Vec2D pos);
	void drawResizingHandles(Vec2D pos);
	void drawRotatingHandles(Vec2D pos);
	void drawItemMenu(double ex);
	void drawSaveMenu(double ex);
	void drawOptionsMenu(double ex);
	void drawIngameUI(double ex);
	void clickHandles(double x, double y, int action);
	void dragClicked(Vec2D world, int action);
	void moveClicked(Vec2D world, int action);
	void resizeClicked(Vec2D world, int action);
	void rotateClicked(Vec2D world, int action);
	void deleteClicked(Vec2D world, int action);
	void copyClicked(Vec2D world, int action);
	void cloneEntity(Entity* e);
	void clonePlatform(Platform* p);
};

