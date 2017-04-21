#include "ParticleSystem.h"
#include "Globals.h"


ParticleSystem::ParticleSystem() {
	for (int i = 0; i < MAX_PARTICLES; i++) {
		particles[i].age = -1;
	}
	particlesPerSecond = 5000;
	lastAddedParticle = 0;
	minVel = 0;
	maxVel = 1;
	minAngle = 0;
	maxAngle = 360;
	angle = 0;
	width = 0;
	height = 0;
	dX = Vec2D(1, 0);
	dY = Vec2D(0, 1);
	emitting = true;
	minLife = 1;
	maxLife = 1;
	minSize = 0.1;
	maxSize = 0.1;
	grav = Vec2D(0, 0);
	vertices.assign(MAX_PARTICLES * 8, 0);
	colors.assign(MAX_PARTICLES * 16, 0);
	tex.assign(MAX_PARTICLES * 8, 0);
}


ParticleSystem::~ParticleSystem() {
}


// Sets the texture of the particles
void ParticleSystem::setTexture(string textureString) {
	texId = ImageLoader::getImage(textureString);
}


// Sets the start position of the particles
void ParticleSystem::setPosition(Vec2D pos) {
	this->pos = pos;
}


// Draws the particle system elapsed seconds after the last draw
void ParticleSystem::draw(double elapsed) {
	lastAddedParticle += elapsed;
	//Add new particles, stopping if max particles is reached
	unsigned int toAdd = (int)(particlesPerSecond  * lastAddedParticle);
	if (toAdd > 0) {
		lastAddedParticle = 0;
	}
	unsigned int totalAlive = 0;
	for (unsigned int i = 0; i < MAX_PARTICLES; i++) {
		if (particles[i].age > 0.0) {
			particles[i].age -= elapsed;
		}
		if (emitting && toAdd > 0 && particles[i].age < 0.0) {
			newParticle(i);
			toAdd--;
		}
		//Add particle to the buffers
		if (particles[i].age > 0.0) {
			//Update particle
			particles[i].vel.addTo(grav.multiply(elapsed));
			particles[i].pos.addTo(particles[i].vel.multiply(elapsed));
			//Position buffer
			GLfloat x = (GLfloat)particles[i].pos.getX();
			GLfloat y = (GLfloat)particles[i].pos.getY();
			GLfloat s = (GLfloat)particles[i].size / 2.0f;
			//TODO: rotation?
			vertices[totalAlive * 8 + 0] = x - s;
			vertices[totalAlive * 8 + 1] = y - s;
			vertices[totalAlive * 8 + 2] = x + s;
			vertices[totalAlive * 8 + 3] = y - s;
			vertices[totalAlive * 8 + 4] = x + s;
			vertices[totalAlive * 8 + 5] = y + s;
			vertices[totalAlive * 8 + 6] = x - s;
			vertices[totalAlive * 8 + 7] = y + s;
			//Texture buffer
			tex[totalAlive * 8 + 0] = 0;
			tex[totalAlive * 8 + 1] = 0;
			tex[totalAlive * 8 + 2] = 1;
			tex[totalAlive * 8 + 3] = 0;
			tex[totalAlive * 8 + 4] = 1;
			tex[totalAlive * 8 + 5] = 1;
			tex[totalAlive * 8 + 6] = 0;
			tex[totalAlive * 8 + 7] = 1;
			//Colour buffer
			GLubyte r, g, b, a;
			//Interpolate the colours
			Color c = presetColors[particles[i].color];
			double t = (particles[i].maxAge - particles[i].age)/particles[i].maxAge;
			r = c.sR + (int)((c.eR - c.sR) * t);
			g = c.sG + (int)((c.eG - c.sG) * t);
			b = c.sB + (int)((c.eB - c.sB) * t);
			a = c.sA + (int)((c.eA - c.sA) * t);
			for (int i = 0; i < 4; i++) {
				colors[totalAlive * 16 + 0 + (i * 4)] = r;
				colors[totalAlive * 16 + 1 + (i * 4)] = g;
				colors[totalAlive * 16 + 2 + (i * 4)] = b;
				colors[totalAlive * 16 + 3 + (i * 4)] = a;
			}
			totalAlive++;
		}
	}
	//Standard texture binding
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	//Tell GPU to use buffers
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	//Send buffers to GPU
	glVertexPointer(2, GL_FLOAT, 0, vertices.data());
	glTexCoordPointer(2, GL_FLOAT, 0, tex.data());
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors.data());
	//Draw particles
	glDrawArrays(GL_QUADS, 0, totalAlive * 4);
	//Disable use of buffers + textures
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisable(GL_TEXTURE_2D);
#ifdef DEBUG
	if (totalAlive > 0) {
		glColor3ub(255, 127, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2d(pos.getX() - 0.1, pos.getY() - 0.1);
		glVertex2d(pos.getX() + 0.1, pos.getY() - 0.1);
		glVertex2d(pos.getX() + 0.1, pos.getY() + 0.1);
		glVertex2d(pos.getX() - 0.1, pos.getY() + 0.1);
		glEnd();
	}
#endif
}


void inline ParticleSystem::newParticle(int i) {
	Particle& p = particles[i];
	double rX = randD(-width / 2, width / 2);
	double rY = randD(-height / 2, height / 2);
	p.pos = pos.add(dX.multiply(rX)).add(dY.multiply(rY));
	p.size = (GLfloat)(randD(minSize, maxSize));
	p.age = randD(minLife,maxLife);
	p.maxAge = p.age;
	p.angle = randD(minAngle, maxAngle);
	p.vel = Vec2D(cos(p.angle * DEG_TO_RAD), sin(p.angle * DEG_TO_RAD));
	p.vel.multiplyBy(randD(minVel, maxVel));
	p.color = rand() % presetColors.size();
}


// Sets the minimum angle the particles travel in
void ParticleSystem::setMinAngle(double angle) {
	this->minAngle = angle;
}


// Sets the maximum angle the particles travel in
void ParticleSystem::setMaxAngle(double angle) {
	this->maxAngle = angle;
}


// Sets the width in which particles can spawn
void ParticleSystem::setWidth(double width) {
	this->width = width;
	dX = Vec2D(cos(angle * DEG_TO_RAD), sin(angle * DEG_TO_RAD));
	dY = Vec2D(-dX.getY(), dX.getX());
}


// Sets the width in which particles can spawn
void ParticleSystem::setHeight(double height) {
	this->height = height;
	dX = Vec2D(cos(angle * DEG_TO_RAD), sin(angle * DEG_TO_RAD));
	dY = Vec2D(-dX.getY(), dX.getX());
}


// Sets the angle of the emitter
void ParticleSystem::setAngle(double angle) {
	this->angle = angle;
	dX = Vec2D(cos(angle * DEG_TO_RAD), sin(angle * DEG_TO_RAD));
	dY = Vec2D(-dX.getY(), dX.getX());
	dX.multiplyBy(randD(-width / 2, width / 2));
	dY.multiplyBy(randD(-height / 2, height / 2));
}


// Sets the minimum velocity of the particles
void ParticleSystem::setMinVelocity(double vel) {
	this->minVel = vel;
}


// Sets the maximum velocity of the particles
void ParticleSystem::setMaxVelocity(double vel) {
	this->maxVel = vel;
}


// Sets whether the particle system emits new particles
void ParticleSystem::setEmitting(bool emitting) {
	this->emitting = emitting;
}


// Sets the number of particles emitted per second
void ParticleSystem::setParticlesPerSecond(int pps) {
	particlesPerSecond = pps;
}


// Sets the minimum time a particle can exist for
void ParticleSystem::setMinLife(double life) {
	this->minLife = life;
}


// Sets the maximum time a particle can exist for
void ParticleSystem::setMaxLife(double life) {
	this->maxLife = life;
}


// Sets the minimum size of a particle
void ParticleSystem::setMinSize(double size) {
	minSize = size;
}


// Sets the maximum size of a particle
void ParticleSystem::setMaxSize(double size) {
	maxSize = size;
}


// Adds a pair of colours a particle can be
void ParticleSystem::addColor(unsigned char sR, unsigned char sG, 
	unsigned char sB, unsigned char sA, unsigned char eR, 
	unsigned char eG,unsigned char eB, unsigned char eA) {
	Color c = Color();
	c.sR = sR;
	c.sG = sG;
	c.sB = sB;
	c.sA = sA;
	c.eR = eR;
	c.eG = eG;
	c.eB = eB;
	c.eA = eA;
	presetColors.push_back(c);
}

// Sets the gravity
void ParticleSystem::setGravity(Vec2D grav) {
	this->grav = grav;
}
