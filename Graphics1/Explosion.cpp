#include "Explosion.h"
#include "Level.h"


Explosion::Explosion() {
	id = "explosion";
	width = 1;
	height = 1;
	age = 1;
	lastTime = 0;
	time = 0;
	ps.setTexture("Resources\\particles\\explosion.png");
	ps.setParticlesPerSecond(MAX_PARTICLES);
	ps.addColor(255, 0, 0, 255, 255, 0, 0, 0);
	ps.addColor(255, 127, 0, 255, 255, 127, 0, 0);
	ps.addColor(255, 255, 0, 255, 255, 255, 0, 0);
	ps.addColor(0, 0, 0, 255, 0, 0, 0, 0);
	ps.setMinLife(0);
	ps.setMaxLife(age);
	ps.setMinVelocity(0);
	ps.setMaxVelocity(3 * (width / age) / 2);
	ps.setMinSize(0.05);
	ps.setMaxSize(0.1);
	ps.setEmitting(true);
	warmed = false;
	explodeSound = SoundLoader::getSound("Resources\\sounds\\explode.wav");
}


Explosion::~Explosion() {
	alDeleteSources(1, &explodeSound);
}


// Sets the width of the explosion
void Explosion::setRadius(double r) {
	this->width = r;
	this->height = r;
	ps.setMinVelocity(0);
	ps.setMaxVelocity((width / age));
}


// Gets the normal vectors needed to check collision
Vec2D* Explosion::getNormals(int* numNormals) {
	//For simplicity explosions are octagons that cannot rotate
	*numNormals = 4;
	Vec2D* r = new Vec2D[4];
	r[0] = Vec2D(1, 0);
	r[1] = Vec2D(0, 1);
	r[2] = Vec2D(ONE_OVER_RT2, ONE_OVER_RT2);
	r[3] = Vec2D(-ONE_OVER_RT2, ONE_OVER_RT2);
	return r;
}


// Gets the vertices of the bounding box
Vec2D* Explosion::getVertices(int* numVertices) {
	*numVertices = 8;
	Vec2D* r = new Vec2D[8];
	//Due to the lack of rotation I can hard code the directions
	//and simply multiply by the radius
	double cos225 = 0.92387953251 * width;
	double sin225 = 0.38268343236 * width;
	//22.5 deg (anti clockwise)
	r[0] = Vec2D(-sin225, cos225).add(pos);
	//67.5 deg (anti clockwise)
	r[1] = Vec2D(-cos225, sin225).add(pos);
	//112.5 deg (anti clockwise)
	r[2] = Vec2D(-cos225, -sin225).add(pos);
	//157.5 deg (anti clockwise)
	r[3] = Vec2D(-sin225, -cos225).add(pos);
	//202.5 deg (anti clockwise)
	r[4] = Vec2D(sin225, -cos225).add(pos);
	//247.5 deg (anti clockwise)
	r[5] = Vec2D(cos225, -sin225).add(pos);
	//292.5 deg (anti clockwise)
	r[6] = Vec2D(cos225, sin225).add(pos);
	//337.5 deg (anti clockwise)
	r[7] = Vec2D(sin225, cos225).add(pos);
	return r;
}


//Updates the entity
void Explosion::update() {
	if (time == 0) {
		alSourcePlay(explodeSound);
	}
	time += TICKRATE;
	if (time >= age) {
		Level* l = (Level*)state;
		l->safeDelete(this);
	}
}


// Called when a collision occurs
void Explosion::onCollide(Collider* other) {
	if (time < TICKRATE) {
		other->onDamage(Damage::EXPLOSION);
	}
}


// Sets the max age of the explosion
void Explosion::setMaxAge(double age) {
	this->age = age;
	time = 0;
	lastTime = 0;
	ps.setMinLife(0);
	ps.setMaxLife(age);
	ps.setMaxVelocity((width / age));
}


void Explosion::draw(double ex) {
	ps.setPosition(pos);
	if (!warmed) {
		ps.preWarm(1);
		warmed = true;
	}
	time += ex;
	if (time > EXPLOSION_NEW_PARTICLES_DURATION) {
		ps.setEmitting(false);
	}
	ps.draw(time - lastTime);
	lastTime = time;
	time -= ex;
#ifdef DEBUG
	//Draw hitbox
	//Note: The laggy appearance of the hitbox is because the game runs at 20hz
	//and is made to appear smoother by extrapolating graphical positions when
	//the game is drawn
	glColor3ub(onGround ? 0 : 255, 127, 0);
	glLineWidth(2);
	int n;
	Vec2D* vecs = getVertices(&n);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < n; i++) {
		glVertex2d(vecs[i].getX(), vecs[i].getY());
	}
	glEnd();
#endif
}
