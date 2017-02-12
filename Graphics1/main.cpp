#include <iostream>
#include <Image_Loading\glew.h>
#include <GLFW\glfw3.h>
#include "Globals.h"
#include "KeyConfig.h"
#include "Level.h"
#include "Entity.h"
#include "Player.h"
#include "ImageLoader.h"
/*
Big list of things to do:
Implement acceleration to entities to improve prediction
Fix Level to return player object + handle camera + stuff
Broad collision detection
Graphics/Animations
Cleanup code (auto generated classes, import orders, etc)
Start and end points
UI
Level editor
AI
Multiple levels
Level loading
Backgrounds
Every stupid thing on the mark sheet
*/
//----------------------------------Globals----------------------------------//
enum GameState { GAME_MENU, GAME_PLAYING }; //The different states of the game
GLFWwindow* gameWindow;
Entity* test;
Level* level;
int skips = 0;
int UPDATESKIPS = 0;
int sWidth;
int sHeight;
//-------------------Callback functions and state handlers-------------------//
///Handles key presses
void keyHandler(GLFWwindow* window, int key, int scan, int action, int mods) {
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		UPDATESKIPS -= 100;
		UPDATESKIPS *= -1;
	}
}
///Handles mouse buttons
void mouseHandler(GLFWwindow* window, int button, int action, int mods) {

}
///Starts the game and loads anything that needs loading
void init() {
	//Load the key bindings
	KeyConfig::loadBindings();
	//Allow KeyConfig to get key states
	KeyConfig::win = gameWindow;
	//Initialise the texture loader
	ImageLoader::makeMissingTexture();
	//Load the player (TEMP)
	level = new Level();
	level->loadLevel("temp");
	test = level->getPlayer();
	//GL Alpha channel stuff
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Default image settings
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}
///Draws the scene
void draw(double ex) {
	//For the sake of checking the background is correct clear to purple
	glClearColor(1.0, 0.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	level->draw(ex);
	glPopMatrix();
}
///Updates the game
void update() {
	level->update();
}
///Resizes the window
void resize(GLFWwindow* window, int width, int height) {
	//Fix the viewport
	//TODO: Maybe fix for different resolutions
	sWidth = width;
	sHeight = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Invert y-axis for screen coords
	glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
//-----------------------Window creation and game loop-----------------------//
int main() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialise GLFW";
		exit(EXIT_FAILURE);
	}
	//GLFW needs to know which version of opengl I am targeting
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	gameWindow = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, TITLE, NULL, NULL);
	if (!gameWindow) {
		std::cerr << "Failed to create window";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(gameWindow);
	//I am given to understand this is glfw's way of enabling v-sync
	glfwSwapInterval(1);
	//Handle window events
	glfwSetWindowSizeCallback(gameWindow, resize);
	//Call resize to initialise display
	resize(gameWindow, DEFAULT_WIDTH, DEFAULT_HEIGHT);
	glfwSetKeyCallback(gameWindow, keyHandler);
	glfwSetMouseButtonCallback(gameWindow, mouseHandler);
	//Initialise the game
	init();
	//Game loop, calls update every TICKRATE ms and renders whenever it can
	//I didn't invent this type of main loop, I found it online a while ago
	//I did, however, implement it
	double previous = glfwGetTime();
	double lastUpdate = 0.0f;
	while (!glfwWindowShouldClose(gameWindow)) {
		//Calculate time taken for last frame
		double current = glfwGetTime();
		double elapsed = current - previous;
		previous = current;
		//Track total time since last update
		lastUpdate += elapsed;
		//Do any input things here
		glfwPollEvents();
		//Call updates until the game has caught up
		//But don't update if not enough time has elapsed
		while (lastUpdate >= TICKRATE) {
			skips++;
			if (skips >= UPDATESKIPS) {
				update();
				skips = 0;
			}
			lastUpdate -= TICKRATE;
		}
		//Draw the game, allowing for any extrapolation
		draw(lastUpdate);
		//Display the back buffer
		glfwSwapBuffers(gameWindow);
	}
	//Shutdown properly
	glfwDestroyWindow(gameWindow);
	glfwTerminate();
}