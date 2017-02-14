#include "Level.h"
#include "Globals.h"
#include "Player.h"

//The cosine of the angle between collision and vector and ground needed to count as standing on it
#define COS_GROUND_ANGLE_MIN -1
#define COS_GROUND_ANGLE_MAX -0.707106

Level::Level() {
	planet = ImageLoader::getImage("Resources\\planet.png");
	stars = ImageLoader::getImage("Resources\\stars.png");
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
			if (intersects(e, p, &res) && res.magnitudeSquare()>FLOAT_ZERO) {
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


// Draws the level
void Level::draw(double ex) {
	double pX = player->getX() + player->getVelX()*ex;
	double pY = player->getY() + player->getVelY()*ex;
	/*
	Camera displays a box of fixed world size (to be determined)
	If screen is larger then box is scaled to fit
	If resolution is wrong (most likely the case) the smallest dimension is box size
	*/
	float resize = WORLD_SIZE / (float)(sWidth < sHeight ? sWidth : sHeight);
	//Move the camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, sWidth * resize, 0.0, sHeight * resize, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glTranslated(0.5 * sWidth * resize, 0.5 * sHeight * resize, 0.0);
	glRotated(-player->updatedVisAngle(ex), 0.0, 0.0, 1.0);
	glTranslated(-pX, -pY, 0.0);
	//Draw the background
	glPushMatrix();
	double pHeight = sqrt(sWidth * sWidth + sHeight * sHeight) * resize;
	glTranslated(pX - pHeight, pY - pHeight * 0.5, 0.0);
	glEnable(GL_TEXTURE_2D);
	//Draw the stars
	glBindTexture(GL_TEXTURE_2D, stars);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(0.0, 0.0);
	glTexCoord2d(4.0, 0.0);
	glVertex2d(pHeight * 2, 0.0);
	glTexCoord2d(4.0, 2.0);
	glVertex2d(pHeight * 2, pHeight);
	glTexCoord2d(0.0, 2.0);
	glVertex2d(0.0, pHeight);
	glEnd();
	//Draw the planet
	glBindTexture(GL_TEXTURE_2D, planet);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(0.0, 0.0);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(pHeight * 2, 0.0);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(pHeight * 2, pHeight * 0.75);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(0.0, pHeight * 0.75);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//Draw the parallax backing

	//Draw the gravity fields
	for (GravityField* f : gravFields) {
		double w = f->width / 2;
		double h = f->height / 2;
		glPushMatrix();
		glTranslated(f->pos.getX(), f->pos.getY(), 0.0);
		//0 degrees should be down, rather than right
		glRotated(f->rotation, 0, 0, 1);
		glBegin(GL_QUADS);
		glColor3ub(0, 0, 255);
		glVertex2d(-1 * w, -1 * h);
		glVertex2d(w, -1 * h);
		glColor4ub(0, 0, 0, 0);
		glVertex2d(w, h);
		glVertex2d(-1 * w, h);
		glEnd();
		glPopMatrix();
	}
	//Draw the platforms
	for (Platform* p : platforms) {
		p->draw(ex);
	}
	//Draw the entities
	for (Entity* e : entities) {
		e->draw(ex);
	}
	glColor3ub(0, 0, 0);
}


// Loads a level from the given file
void Level::loadLevel(string filePath) {
	//TEMP TESTING
	GravityField* t = new GravityField();
	t->height = 450;
	t->width = 250;
	t->strength = 2;
	t->pos = Vec2D(400, 100);
	t->rotation = 45;
	gravFields.push_back(t);
	Platform* p = new Platform();
	p->setPos(Vec2D(400, -100));
	p->setWidth(800);
	p->setHeight(200);
	p->setAngle(0);
	platforms.push_back(p);
	p = new Platform();
	p->setPos(Vec2D(400, -350));
	p->setWidth(1000);
	p->setHeight(200);
	p->setAngle(0);
	platforms.push_back(p);
	//Might keep this, make player first entity
	player = new Player();
	player->setLevel(this);
	player->setX(400);
	player->setY(300);
	entities.push_back(player);
}

// Calculates the force of gravity applied to an object at a location
void Level::getGravityAtPos(Vec2D pos, Vec2D* grav) {
	grav->set(0, 0);
	for (GravityField* f : gravFields) {
		//Translate point to be relative to the field's centre
		Vec2D p = pos.subtract(f->pos);
		//Rotate the point back to be AA with the field (-angle)
		double cTheta = cos(-DEG_TO_RAD * f->rotation); //Slight optimisation as trig is expensive
		double sTheta = sin(-DEG_TO_RAD * f->rotation);
		double xPrime = p.getX() * cTheta - p.getY() * sTheta;
		double yPrime = p.getY() * cTheta + p.getX() * sTheta;
		//Calculate field strength at point
		if (xPrime >= -f->width / 2 && xPrime <= f->width / 2
			&& yPrime >= -f->height / 2 && yPrime <= f->height / 2) {
			//Rotate this strength back and add it to force
			//(Forces have been rotated through 90 degrees to make 0 rotation = normal gravity
			grav->addTo(Vec2D(
				f->strength * sin(DEG_TO_RAD * (180-f->rotation)),
				f->strength * cos(DEG_TO_RAD * (180-f->rotation))
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
		}
		else {
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
		}
		else if (p > *max) {
			*max = p;
		}
	}
}


// Gets the player entity
Player* Level::getPlayer() {
	return (Player*)entities.at(0);
}
