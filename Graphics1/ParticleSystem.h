#pragma once
#include "Globals.h"
#define MAX_PARTICLES 1000

class ParticleSystem {
public:
	ParticleSystem();
	~ParticleSystem();
	// Sets the texture of the particles
	void setTexture(string textureString);
	// Sets the start position of the particles
	void setPosition(Vec2D pos);
	// Draws the particle system elapsed seconds after the last draw
	void draw(double elapsed);
protected:
	Vec2D pos;
	GLuint texId;
	struct Particle {
		Vec2D pos;
		Vec2D vel;
		unsigned char r, g, b, a;
		GLfloat size;
		double angle;
		double age;
	};
	Particle particles[MAX_PARTICLES];
	int particlesPerSecond;
	double lastAddedParticle;
	void inline newParticle(int i);
	GLfloat vertices[MAX_PARTICLES * 8];
	GLubyte colors[MAX_PARTICLES * 4];
	GLfloat tex[MAX_PARTICLES * 8];
};

