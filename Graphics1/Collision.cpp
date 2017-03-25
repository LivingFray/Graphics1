#include "Collision.h"


// Calculates if two colliders are intersecting and provides the vector to move one in if so
bool Collision::intersects(Collider* a, Collider* b, Vec2D* res) {
	//Get a list of each vector to project onto
	vector<Vec2D> normals;
	int num;
	Vec2D* n = a->getNormals(&num);
	for (int i = 0; i < num; i++) {
		normals.push_back(n[i]);
	}
	n = b->getNormals(&num);
	for (int i = 0; i < num; i++) {
		normals.push_back(n[i]);
	}
	//Check each normal for intersection
	double smallestMag = HUGE;
	for (Vec2D n : normals) {
		n.toUnit();
		double aMin, aMax, bMin, bMax;
		project(a, n, &aMin, &aMax);
		project(b, n, &bMin, &bMax);
		if (aMax < bMin || bMax < aMin) {
			return false;
		}
		//Calculate the distance needed to move the object
		double d;
		if (aMax < bMax) {
			d = bMin - aMax;
		} else {
			d = bMax - aMin;
		}
		//Check if smallest vector so far
		if (abs(d) < abs(smallestMag)) {
			smallestMag = d;
			*res = n;
		}
	}
	res->multiplyBy(smallestMag);
	return true;
}


// Projects an object onto a vector
void Collision::project(Collider* c, Vec2D vec, double* min, double* max) {
	int n;
	Vec2D* vertices = c->getVertices(&n);
	*min = vertices[0].dot(vec);
	*max = *min;
	for (int i = 1; i < n; i++) {
		double p = vertices[i].dot(vec);
		if (p < *min) {
			*min = p;
		} else if (p > *max) {
			*max = p;
		}
	}
}


// Calculates the distance between two colliders along one vector
double Collision::distance(Collider* a, Collider* b, Vec2D n) {
	n.toUnit();
	double aMin, aMax, bMin, bMax;
	Collision::project(a, n, &aMin, &aMax);
	Collision::project(b, n, &bMin, &bMax);
	if (aMax < bMax) {
		return bMin - aMax;
	}
	return bMax - aMin;
}


//Performs a rough check to see if the colliders have collided
bool Collision::broadCheck(Collider* a, Collider* b) {
	//If both are stationary, assume no collsion
	if (a->getVel().subtract(b->getVel()).magnitudeSquare() < FLOAT_ZERO) {
		return false;
	}
	//Perform circle check using circles that contain the hitboxes
	double w1, w2, h1, h2;
	w1 = a->getWidth();
	w2 = b->getWidth();
	h1 = a->getHeight();
	h2 = b->getHeight();
	//Square values
	w1 *= w1;
	w2 *= w2;
	h1 *= h1;
	h2 *= h2;
	//Sum of the radii squared
	double radii = w1 + h1 + w2 + h2;
	return radii > a->getPos().subtract(b->getPos()).magnitudeSquare();
}

//New collision method:
//Calculate new positions
//Resolve collisions (moving back in vel)
//Move remainder of update (ignores secondary collisions, but should be fine)

//Handles collisions between two objects
void Collision::handle(Level* l, Entity* a, Platform* b, bool &onGround) {
	//Perform a broad check
	if (!Collision::broadCheck(a, b)) {
		return;
	}
	Vec2D res;
	//Use SAT to check for collisions
	if (Collision::intersects(a, b, &res) && res.magnitudeSquare() > FLOAT_ZERO) {
		//Move outside of collision
		a->addPosX(res.getX());
		a->addPosY(res.getY());
		//Arrest velocity
		//This took me longer to do than it took to implement the rest
		//of the entire collision detection system...
		Vec2D vel = a->getVel();
		if (vel.magnitudeSquare() > FLOAT_ZERO) {
			//Only remove in direction of response
			//cos(theta) = (res).(-vel) / (|res||vel|)
			//newResponse = unit(response) * |vel|cos(theta)
			//scaledResponse = oldVel * cos(theta)
			//newVel = oldVel - scaledResponse
			double cTheta = (res.dot(vel.multiply(-1))) / (res.magnitude() * vel.magnitude());
			vel.addTo(res.unit().multiply(vel.magnitude() * cTheta));
			//vel.addTo(vel.multiply(-cTheta));
			a->setVel(vel);
		}
		Vec2D grav;
		l->getGravityAtPos(a->getPos(), &grav);
		//Check resolution vector is in angle range to suggest floor
		/*
		a.b = |a||b|cos(theta)
		grav.res = |grav||res|cos(theta)
		if cos(theta)<cos(45) onGround
		cos(theta) = grav.res / (|grav||res|)
		*/
		if (grav.magnitudeSquare() > FLOAT_ZERO) {
			double cosAngle = grav.dot(res) / (grav.magnitude() * res.magnitude());
			if (cosAngle >= COS_GROUND_ANGLE_MIN && cosAngle <= COS_GROUND_ANGLE_MAX) {
				onGround = true;
			}
		}
		//Handle other collisiony things
		a->onCollide(b);
		b->onCollide(a);
	}
}