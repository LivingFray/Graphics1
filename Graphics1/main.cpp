#include <iostream>
#include <GLFW\glfw3.h>
#include "KeyConfig.h"

//----------------------------------Globals----------------------------------//
const double TICKRATE = 0.05; //The time between update "ticks" in seconds
const char* TITLE = "Gravitate (Title WIP)"; //The game's title
enum GameState {GAME_MENU, GAME_PLAYING}; //The different states of the game
//-------------------Callback functions and state handlers-------------------//
///Handles key presses
void keyHandler(GLFWwindow* window, int key, int scan, int action, int mods) {

}
///Handles mouse buttons
void mouseHandler(GLFWwindow* window, int button, int action, int mods) {

}
///Starts the game and loads anything that needs loading
void start() {
	//Load the key bindings
	keyconfig::loadBindings();
}
///Draws the scene
void draw(double ex) {

}
///Updates the game
void update() {

}
//-----------------------Window creation and game loop-----------------------//
int main() {
	GLFWwindow* window;
	if (!glfwInit()) {
		std::cerr << "Failed to initialise GLFW";
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(800, 600, TITLE, NULL, NULL);
	if (!window) {
		std::cerr << "Failed to create window";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, keyHandler);
	glfwSetMouseButtonCallback(window, mouseHandler);
	start();
	//Game loop, calls update every 1/TPS ms and renders whenever it can
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
		//Display
		glfwSwapBuffers(window);
	}
}