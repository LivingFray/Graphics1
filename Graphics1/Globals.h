#pragma once
#include <string>
#include <map>
#include <vector>
#include "FreeType.h"
#include "DataObject.h"
#include "ImageLoader.h"
#include "KeyConfig.h"
//The time in seconds between updates
#define TICKRATE 0.05
//The games title
#define TITLE "Gravitate (Title WIP)"
//The width of the game window on launch
#define DEFAULT_WIDTH 800
//The height of the game window on launch
#define DEFAULT_HEIGHT 600
//Constant to multiply an angle by to get it in radians
#define DEG_TO_RAD 0.01745329251
//Constant to multiply an angle by to get it in degrees
#define RAD_TO_DEG 57.2957795131
//Small value to compare floating points to zero with
#define FLOAT_ZERO 1e-5
//How many units in world size the smallest side of the screen sees
#define WORLD_SIZE 800
//How many degrees an entity can turn per second (visually) to match gravity
#define ENTITY_ROTATE_SPEED 180
//The default font size used
#define FONT_SIZE_LARGE 24
//The default smaller font size used
#define FONT_SIZE_SMALL 12
//Useful angles
#define PI 3.14159265359
#define TAU 6.28318530718
extern GLFWwindow* gameWindow;
extern int sWidth;
extern int sHeight;
extern freetype::font_data fontLarge;
extern freetype::font_data fontSmall;
class BaseState;
extern BaseState* state;
extern BaseState* newState;
extern GLFWcursor* cursorNormal;
extern GLFWcursor* cursorPan;