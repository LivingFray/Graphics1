#include "Animation.h"



Animation::Animation() {
	spritesheet = ImageLoader::getImage("error");
	frame = 0;
	elapsed = 0.0;
	repeat = true;
	numX = 1;
	numY = 1;
	width = 0.0;
	height = 0.0;
	duration = 0.0;
	frameStart = 0.0;
	pos = Vec2D(0.0, 0.0);
}


Animation::~Animation() {
}


// Sets the texture ID of the spritesheet
void Animation::setSpriteSheet(string texture) {
	spritesheet = ImageLoader::getImage(texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


// Gets the texture ID of the spritesheet
GLuint Animation::getSpritesheet() {
	return spritesheet;
}


// Adds a frame to the animation
void Animation::addFrame(int index, double duration) {
	Frame f = Frame();
	f.duration = duration;
	this->duration += duration;
	f.index = index;
	frames.push_back(f);
}


// Sets whether the animation repeats or stays on the final frame
void Animation::setRepeat(bool repeat) {
	this->repeat = repeat;
}


// Sets how far along the animation has progressed
void Animation::setTime(double time) {
	//Keep the time within the bounds of the animation
	if (repeat) {
		elapsed = time - duration * floor(time / duration);
	} else if (time > duration) {
		elapsed = duration;
	} else {
		elapsed = time;
	}
	//If the frame is before the current one
	if (elapsed < frameStart) {
		frameStart = 0;
		frame = 0;
	}
	//If the frame is after the current one
	if (elapsed - frameStart > frames[frame].duration) {
		while (frameStart < elapsed) {
			frameStart += frames[frame].duration;
			frame++;
		}
		frame--;
		frameStart -= frames[frame].duration;
	}
}


// Draws the animation
void Animation::draw(double dt) {
	//Update animation
	addTime(dt);
	int f = frames[frame].index;
	glColor3ub(255, 255, 255);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spritesheet);
	//Get texture coordinates of sprite
	double x = (f % numX) / (double)numX;
	//Index is from top left, but OpenGL has the bottom left as 0,0
	double y = 1.0 - (f / numX) / (double)numY;
	//The size of a frame
	double sX = 1.0 / numX;
	double sY = 1.0 / numY;
	y -= sY;
	//Display size of the frame
	double w = width * 0.5;
	double h = height * 0.5;
	//Move to position
	glTranslated(pos.getX(), pos.getY(), 0.0);
	glBegin(GL_QUADS);
	//Bottom left
	glTexCoord2d(x, y);
	glVertex2d(-w, -h);
	//Bottom right
	glTexCoord2d(x + sX, y);
	glVertex2d(w, -h);
	//Top right
	glTexCoord2d(x + sX, y + sY);
	glVertex2d(w, h);
	//Top left
	glTexCoord2d(x, y + sY);
	glVertex2d(-w, h);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	setTime(elapsed - dt);
}


// Gets how far along the animation has progressed
double Animation::getTime() {
	return elapsed;
}


// Adds to how far along the animation has progressed
void Animation::addTime(double dt) {
	setTime(elapsed + dt);
}


// Sets how many frames fit in the spritesheet in each direction
void Animation::setSpritesheetSize(int x, int y) {
	numX = x;
	numY = y;
}


// Sets the size of the animation when it is drawn
void Animation::setSize(double width, double height) {
	this->width = width;
	this->height = height;
}


// Sets the position of the animation
void Animation::setPos(Vec2D pos) {
	this->pos = pos;
}


// Sets the width of the animation
void Animation::setWidth(double width) {
	this->width = width;
}


// Sets the height of the animation
void Animation::setHeight(double height) {
	this->height = height;
}


// Sets the width of the animation
double Animation::getWidth() {
	return width;
}


// Sets the height of the animation
double Animation::setHeight() {
	return height;
}
