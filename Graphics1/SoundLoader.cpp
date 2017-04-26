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
	//Pre load sounds to prevent game hanging before playing sounds
	getSound("Resources\\sounds\\spawn.wav", true);
	getSound("Resources\\sounds\\goal.wav", true);
	getSound("Resources\\sounds\\tick.wav", true);
	getSound("Resources\\sounds\\explode.wav", true);
	getSound("Resources\\sounds\\spark.wav", true);
	getSound("Resources\\sounds\\fire.wav", true);
}


// Returns an openal sound, loading from file if need be
ALuint SoundLoader::getSound(string path, bool preload) {
	if (sounds[path] != NULL) {
		ALuint source;
		alGenSources(1, &source);
		alSourcei(source, AL_BUFFER, sounds[path]);
		return source;
	}
	ALuint buffer = alutCreateBufferFromFile(path.c_str());
	if (buffer == AL_NONE) {
		return NULL;
	}
	sounds[path] = buffer;
	if (preload) {
		printf(("Preloaded " + path + "\n").c_str());
		return NULL;
	}
	ALuint source;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);
	return source;
}


//Frees every image stored
void SoundLoader::cleanup() {
	ALuint* sources = new ALuint[sounds.size()];
	int p = 0;
	for (auto i : sounds) {
		sources[p] = i.second;
		p++;
	}
	alDeleteBuffers(sounds.size(), sources);
	delete sources;
	sounds.clear();
	alutExit();
}
