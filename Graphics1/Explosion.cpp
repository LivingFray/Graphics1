#include "Explosion.h"



Explosion::Explosion() {
	id = "explosion";
	width = 1;
	height = 1;
	currentAnim = Animation();
	currentAnim.setSpriteSheet("Resources\\entities\\explosion.png");
	currentAnim.setSize(width, height);
	currentAnim.setRepeat(false);
	currentAnim.setSpritesheetSize(2, 2);
	currentAnim.addFrame(0, 1);
}


Explosion::~Explosion() {
}


// Sets the width of the explosion
void Explosion::setRadius(double r) {
	this->width = r;
	this->height = r;
	currentAnim.setSize(r * 2, r * 2);
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
}


// Called when a collision occurs
void Explosion::onCollide(Collider* other) {
	other->onDamage(Damage::EXPLOSION);
}
