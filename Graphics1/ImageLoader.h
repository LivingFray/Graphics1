#pragma once
#include "Globals.h"


typedef map<string, GLuint> imageMap;

namespace ImageLoader {
	extern imageMap images;
	extern GLuint missing;
	// Returns an opengl image context, loading image from file if need be
	GLuint getImage(string path);
	// Dynamically generate the classic missing texture checkerboard to fall back on
	void makeMissingTexture();
	//Frees every image stored
	void cleanup();
}