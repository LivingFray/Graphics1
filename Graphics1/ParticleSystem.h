#pragma once
#include "Globals.h"


class ParticleSystem {
public:
	ParticleSystem();
	~ParticleSystem();
	// Sets the texture of the particles
	void setTexture(string textureString);
	// Sets the start position of the particles
	void setPosition(Vec2D pos);
	// Updates the particle system to appear to have already been running
	void preWarm(double elapsed);
	// Draws the particle system elapsed seconds after the last draw
	void draw(double elapsed);
	// Sets the minimum angle the particles travel in
	void setMinAngle(double angle);
	// Sets the maximum angle the particles travel in
	void setMaxAngle(double angle);
	// Sets the width in which particles can spawn
	void setWidth(double width);
	// Sets the width in which particles can spawn
	void setHeight(double height);
	// Sets the angle of the emitter
	void setAngle(double angle);
	// Sets the minimum velocity of the particles
	void setMinVelocity(double vel);
	// Sets the maximum velocity of the particles
	void setMaxVelocity(double vel);
	// Sets whether the particle system emits new particles
	void setEmitting(bool emitting);
	// Sets the number of particles emitted per second
	void setParticlesPerSecond(int pps);
	// Sets the minimum time a particle can exist for
	void setMinLife(double life);
	// Sets the maximum time a particle can exist for
	void setMaxLife(double life);
	// Sets the minimum size of a particle
	void setMinSize(double size);
	// Sets the maximum size of a particle
	void setMaxSize(double size);
	// Adds a pair of colours a particle can be
	void addColor(unsigned char sR, unsigned char sG,
		unsigned char sB, unsigned char sA, unsigned char eR,
		unsigned char eG, unsigned char eB, unsigned char eA);
	// Sets the gravity
	void setGravity(Vec2D grav);
	// Clears the particle system
	void clear();
protected:
	Vec2D pos;
	Vec2D grav;
	GLuint texId;
	struct Particle {
		Vec2D pos;
		Vec2D vel;
		unsigned int color;
		GLfloat size;
		double angle;
		double age;
		double maxAge;
	};
	Particle particles[MAX_PARTICLES];
	int particlesPerSecond;
	double lastAddedParticle;
	void inline newParticle(int i);
	vector<GLfloat> vertices;
	vector<GLubyte> colors;
	vector<GLfloat> tex;
	double minAngle;
	double maxAngle;
	double width;
	double height;
	double angle;
	double minVel;
	double maxVel;
	double minLife;
	double maxLife;
	double minSize;
	double maxSize;
	Vec2D dX, dY;
	bool emitting;
	struct Color {
		unsigned char sR, sG, sB, sA, eR, eG, eB, eA;
	};
	vector<Color> presetColors;
};

