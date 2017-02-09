#pragma once
#include <map>
#include <string>
#include <Image_Loading\glew.h>
#include <GLFW\glfw3.h>

using namespace std;
typedef map<string, GLuint> imageMap;

namespace ImageLoader {
	extern imageMap images;
	extern GLuint missing;
	// Returns an opengl image context, loading image from file if need be
	GLuint getImage(string path);
	void makeMissingTexture();
}