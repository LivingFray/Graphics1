#include <iostream>
#include <GLFW\glfw3.h>
#include "Globals.h"
#include "KeyConfig.h"
#include "Level.h"
#include "Entity.h"
//----------------------------------Globals----------------------------------//
enum GameState { GAME_MENU, GAME_PLAYING }; //The different states of the game
GLFWwindow* window;
Entity* test;
Level* level;
//-------------------Callback functions and state handlers-------------------//
///Handles key presses
void keyHandler(GLFWwindow* window, int key, int scan, int action, int mods) {

}
///Handles mouse buttons
void mouseHandler(GLFWwindow* window, int button, int action, int mods) {

}
///Starts the game and loads anything that needs loading
void init() {
	//Load the key bindings
	keyconfig::loadBindings();
	//Load the player (TEMP)
	test = new Entity();
	test->setX(400);
	test->setY(300);
	level = new Level();
	level->loadLevel("temp");

	//GL Alpha channel stuff
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
///Draws the scene
void draw(double ex) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	level->draw(ex);
	test->draw();
}
///Updates the game
void update() {
	double vX = 0.0;
	double vY = 0.0;
	if (glfwGetKey(window, keyconfig::keyBindings["moveLeft"]) == GLFW_PRESS) {
		vX -= 50.0;
	}
	if (glfwGetKey(window, keyconfig::keyBindings["moveRight"]) == GLFW_PRESS) {
		vX += 50.0;
	}
	if (glfwGetKey(window, keyconfig::keyBindings["dbUp"]) == GLFW_PRESS) {
		vY -= 50.0;
	}
	if (glfwGetKey(window, keyconfig::keyBindings["dbDown"]) == GLFW_PRESS) {
		vY += 50.0;
	}
	double fx = 0.0;
	double fy = 0.0;
	level->getGravityAtPos(test->getX(), test->getY(), &fx, &fy);
	test->addVelX(vX + fx);
	test->addVelY(vY + fy);
	test->update();
	level->update();
}
///Resizes the window
void resize(GLFWwindow* window, int width, int height) {
	//Fix the viewport
	//TODO: Maybe fix for different resolutions
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
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
	window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, TITLE, NULL, NULL);
	if (!window) {
		std::cerr << "Failed to create window";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	//I am given to understand this is glfw's way of enabling v-sync
	glfwSwapInterval(1);
	//Handle window events
	glfwSetWindowSizeCallback(window, resize);
	//Call resize to initialise display
	resize(window, DEFAULT_WIDTH, DEFAULT_HEIGHT);
	glfwSetKeyCallback(window, keyHandler);
	glfwSetMouseButtonCallback(window, mouseHandler);
	//Initialise the game
	init();
	//Game loop, calls update every TICKRATE ms and renders whenever it can
	//I didn't invent this type of main loop, I found it online a while ago
	//I did, however, implement it
	double previous = glfwGetTime();
	double lastUpdate = 0.0f;
	while (!glfwWindowShouldClose(window)) {
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
			update();
			lastUpdate -= TICKRATE;
		}
		//Draw the game, allowing for any extrapolation
		draw(lastUpdate / TICKRATE);
		//Display the back buffer
		glfwSwapBuffers(window);
	}
	//Shutdown properly
	glfwDestroyWindow(window);
	glfwTerminate();
}