#include "ParticleSystem.h"
#define DEBUG


ParticleSystem::ParticleSystem() {
	for (int i = 0; i < MAX_PARTICLES; i++) {
		particles[i].age = -1;
	}
	particlesPerSecond = 500;
	lastAddedParticle = 0;
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
		particles[i].age -= elapsed;
		if (toAdd > 0 && particles[i].age < 0.0) {
			newParticle(i);
			toAdd--;
		}
		//Add particle to the buffers
		if (particles[i].age > 0.0) {
			//Update particle
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
			colors[totalAlive * 4 + 0] = particles[i].r;
			colors[totalAlive * 4 + 1] = particles[i].g;
			colors[totalAlive * 4 + 2] = particles[i].b;
			colors[totalAlive * 4 + 3] = particles[i].a;
			totalAlive++;
		}
	}
	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Standard texture binding
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	//Tell GPU to use buffers
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	//Send buffers to GPU
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT,0, tex);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
	//Draw particles
	glDrawArrays(GL_QUADS, 0, totalAlive * 4);
	//Disable use of buffers + textures
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glPopAttrib();
#ifdef DEBUG
	if (totalAlive > 0) {
		glBegin(GL_POINTS);
		glVertex2d(pos.getX(), pos.getY());
		glEnd();
	}
#endif
}


void inline ParticleSystem::newParticle(int i) {
	Particle& p = particles[i];
	p.pos = pos;
	p.size = 0.1f;
	p.vel = Vec2D(rand() % 2000 / 1000.0 - 1, rand() % 2000 / 1000.0 - 1);
	p.age = 5;
	p.angle = 0;
	//p.r = rand() % 256;
	//p.g = rand() % 256;
	//p.b = rand() % 256;
	p.r = p.g = p.b = 255;
	p.a = 255;
}
