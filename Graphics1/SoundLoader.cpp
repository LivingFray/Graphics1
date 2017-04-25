#include "SoundLoader.h"
//I'll just add sound, I thought. This shouldn't be too hard, I thought.
//Several hours of searching and the only way I could get hold of alut was by
//Going through archive.org and accessing a download page from 2006.

soundMap SoundLoader::sounds;


// Initialises OpenAL and handles any setup
void SoundLoader::init() {
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("Error Initialising sound");
	}
}


// Returns an openal sound, loading from file if need be
ALuint SoundLoader::getSound(string path) {
	if (sounds[path] != NULL) {
		return sounds[path];
	}
	ALuint buffer = alutCreateBufferFromFile(path.c_str());
	if (buffer == AL_NONE) {
		return NULL;
	}
	ALuint source;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);
	sounds[path] = source;
	return source;
}


//Frees every image stored
void SoundLoader::cleanup() {
	ALuint* sources = new ALuint[sounds.size()];
	int p = 0;
	for (auto i : sounds) {
		//Stop any playing sounds
		alSourceStop(i.second);
		sources[p] = i.second;
		p++;
	}
	glDeleteTextures(sounds.size(), sources);
	delete sources;
	sounds.clear();
	alutExit();
}
