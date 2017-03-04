#include "Level.h"
#include "Globals.h"
#include "Player.h"

//The cosine of the angle between collision and vector and ground needed to count as standing on it
#define COS_GROUND_ANGLE_MIN -1
#define COS_GROUND_ANGLE_MAX -0.707106

Level::Level() {

}


Level::~Level() {
	//TODO: Free level data
}


// Updates the level
void Level::update() {
	//TODO: Update world
	//TODO: If entity is still only check against moving platforms
	//Only set onGround to false for entities that moved / floor moved
	//Update the entities
	for (Entity* e : entities) {
		e->update();
		bool onGround = e->getOnGround();
		bool moving = abs(e->getVelX()) > FLOAT_ZERO || abs(e->getVelY()) > FLOAT_ZERO;
		if (moving) {
			onGround = false;
		}
		//TODO: Skip entities that haven't moved
		//TODO: Handle moving platforms once implemented
		//Perform collision detection + resolution
		for (Platform* p : platforms) {
			//TODO: Broad check here

			//Use SAT to check for collisions
			Vec2D res;
			if (intersects(e, p, &res) && res.magnitudeSquare() > FLOAT_ZERO) {
				//Move outside of collision
				e->addPosX(res.getX());
				e->addPosY(res.getY());
				//Arrest velocity
				//This took me longer to do than it took to implement the rest
				//of the entire collision detection system...
				if (moving) {
					//Only remove in direction of response
					Vec2D vel = Vec2D(e->getVelX(), e->getVelY());
					//cos(theta) = (res).(-vel) / (|res||vel|)
					//newResponse = unit(response) * |vel|cos(theta)
					//newVel = oldVel - newResponse
					double cTheta = (res.dot(vel.multiply(-1))) / (res.magnitude() * vel.magnitude());
					//double sTheta = sqrt(1 - cTheta * cTheta);
					vel.addTo(res.unit().multiply(vel.magnitude() * cTheta));
					e->setVelX(vel.getX());
					e->setVelY(vel.getY());
				}
				Vec2D grav;
				getGravityAtPos(e->getPos(), &grav);
				//Check resolution vector is in angle range to suggest floor
				/*
				a.b = |a||b|cos(theta)
				grav.res = |grav||res|cos(theta)
				if cos(theta)<cos(45) onGround
				cos(theta) = grav.res / (|grav||res|)
				*/
				double cosAngle = grav.dot(res) / (grav.magnitude() * res.magnitude());
				if (cosAngle >= COS_GROUND_ANGLE_MIN && cosAngle <= COS_GROUND_ANGLE_MAX) {
					onGround = true;
				}
				//Handle other collisiony things
				e->onCollide(p);
				p->onCollide(e);
			}
		}
		e->setOnGround(onGround);
	}
}


// Loads a level from the given file
void Level::loadLevel(string filePath) {
	//Make player first entity
	player = new Player();
	player->setLevel(this);
	entities.push_back(player);
	LevelRenderer::loadLevel(filePath);
	//TODO: Set player position
	player->setX(100);
}

// Draws the level
void Level::draw(double ex) {
	LevelRenderer::draw(ex);
	//Insert any UI stuff here
}


// Calculates the force of gravity applied to an object at a location
void Level::getGravityAtPos(Vec2D pos, Vec2D* grav) {
	grav->set(0, 0);
	for (GravityField* f : gravFields) {
		//Translate point to be relative to the field's centre
		Vec2D p = pos.subtract(f->getPos());
		//Rotate the point back to be AA with the field (-angle)
		double cTheta = cos(-DEG_TO_RAD * f->getAngle()); //Slight optimisation as trig is expensive
		double sTheta = sin(-DEG_TO_RAD * f->getAngle());
		double xPrime = p.getX() * cTheta - p.getY() * sTheta;
		double yPrime = p.getY() * cTheta + p.getX() * sTheta;
		//Calculate field strength at point
		if (xPrime >= -f->getWidth() / 2 && xPrime <= f->getWidth() / 2
			&& yPrime >= -f->getHeight() / 2 && yPrime <= f->getHeight() / 2) {
			//Rotate this strength back and add it to force
			//(Forces have been rotated through 90 degrees to make 0 rotation = normal gravity
			grav->addTo(Vec2D(
				f->getStrength() * sin(DEG_TO_RAD * (180 - f->getAngle())),
				f->getStrength() * cos(DEG_TO_RAD * (180 - f->getAngle()))
			));
		}
	}
	if (grav->getX() == 0 && grav->getY() == 0) {
		grav->setY(-defaultGravity);
	}
}


// Calculates if two colliders are intersecting and provides the vector to move one in if so
bool Level::intersects(Collider* a, Collider* b, Vec2D* res) {
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
void Level::project(Collider* c, Vec2D vec, double* min, double* max) {
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


// Gets the player entity
Player* Level::getPlayer() {
	return (Player*)entities.at(0);
}


// Gets the camera position ex seconds after last update
Vec2D Level::getCameraAt(double ex) {
	return player->getPos().add(player->getVel().multiply(ex));
}


// Gets the angle of the camera ex seconds after last update
double Level::getCameraAngleAt(double ex) {
	return player->getVisAngle();
}
