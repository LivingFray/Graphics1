#pragma once
#include "Globals.h"

using namespace std;
typedef map<string, GLuint> imageMap;

namespace ImageLoader {
	extern imageMap images;
	extern GLuint missing;
	// Returns an opengl image context, loading image from file if need be
	GLuint getImage(string path);
	void makeMissingTexture();
}