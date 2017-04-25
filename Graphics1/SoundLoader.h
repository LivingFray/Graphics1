#pragma once
#include "Globals.h"
#include <al.h>
#include <alc.h>
#include "alut.h"

typedef map<string, ALuint> soundMap;

namespace SoundLoader {
	extern soundMap sounds;
	// Initialises OpenAL and handles any setup
	void init();
	// Returns an openal sound, loading from file if need be
	ALuint getSound(string path);
	//Frees every sound stored
	void cleanup();
}