#pragma once
#include "Entity.h"
#include "ParticleSystem.h"
class Explosion :
	public Entity {
public:
	Explosion();
	~Explosion();
	// Sets the radius of the explosion
	void setRadius(double r);
	// Gets the normal vectors needed to check collision
	Vec2D* getNormals(int* numNormals);
	// Gets the vertices of the bounding box
	Vec2D* getVertices(int* numVertices);
	// Updates the entity
	void update();
	// Called when a collision occurs
	void onCollide(Collider* other);
	// Sets the age of the explosion
	void setMaxAge(double age);

	void draw(double ex);
protected:
	double age;
	double time;
	double lastTime;
	ParticleSystem ps;
};

