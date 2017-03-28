#pragma once
#include "Globals.h"

class Animation {
public:
	Animation();
	~Animation();
	// Sets the texture ID of the spritesheet
	void setSpriteSheet(string texture);
	// Gets the texture ID of the spritesheet
	GLuint getSpritesheet();
	// Adds a frame to the animation
	void addFrame(int index, double duration);
	// Sets whether the animation repeats or stays on the final frame
	void setRepeat(bool repeat);
	// Sets how far along the animation has progressed
	void setTime(double time);
	// Draws the animation
	void draw(double dt);
	// Gets how far along the animation has progressed
	double getTime();
	// Adds to how far along the animation has progressed
	void addTime(double dt);
	// Sets how many frames fit in the spritesheet in each direction
	void setSpritesheetSize(int x, int y);
	// Sets the size of the animation when it is drawn
	void setSize(double width, double height);
	// Sets the position of the animation
	void setPos(Vec2D pos);
private:
	GLuint spritesheet;
	double elapsed;
	bool repeat;
	int numX;
	int numY;
	double width;
	double height;
	struct Frame {
		int index;
		double duration;
	};
	vector<Frame> frames;
	double frameStart;
	double duration;
	int frame;
	Vec2D pos;
};

