#include "Slope.h"



Slope::Slope() {
	id = "slope";
}


Slope::~Slope() {
}


// Draws the platform ex seconds after last update
void Slope::draw(double ex) {
	glColor3ub(255, 255, 255);
	glPushMatrix();
	glTranslated(pos.getX() + vel.getX()*ex, pos.getY() + vel.getY()*ex, 0.0);
	glRotated(angle, 0.0, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_TRIANGLES);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(-width / 2, -height / 2);
	glTexCoord2d(0.0, texY);
	glVertex2d(-width / 2, height / 2);
	glTexCoord2d(texX, 0.0);
	glVertex2d(width / 2, -height / 2);
	glEnd();
	glDisable(GL_TEXTURE_2D);
#ifdef DEBUG
	//Draw velocity
	glRotated(-angle, 0.0, 0.0, 1.0);
	glColor3ub(0, 0, 255);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2d(0.0, 0.0);
	glVertex2d(vel.getX(), vel.getY());
	glEnd();
	//Draw hitbox
	//Note: The laggy appearance of the hitbox is because the game runs at 20hz
	//and is made to appear smoother by extrapolating graphical positions when
	//the game is drawn
	glTranslated(-pos.getX() - vel.getX()*ex, -pos.getY() - vel.getY()*ex, 0.0);
	glColor3ub(255, 127, 0);
	glLineWidth(2);
	int n;
	Vec2D* vecs = getVertices(&n);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < n; i++) {
		glVertex2d(vecs[i].getX(), vecs[i].getY());
	}
	delete[] vecs;
	glEnd();
	//Draw Normals
	Vec2D* norms = getNormals(&n);
	glColor3ub(0, 255, 255);
	glBegin(GL_LINES);
	for (int i = 0; i < n; i++) {
		glVertex2d(pos.getX(), pos.getY());
		glVertex2d(pos.getX() + norms[i].getX(), pos.getY() + norms[i].getY());
	}
	delete[] norms;
	glEnd();
#endif
	glPopMatrix();
}


// Gets the normal vectors needed to check collision
Vec2D* Slope::getNormals(int* numNormals) {
	*numNormals = 3;
	Vec2D* r = new Vec2D[3];
	//Calculate the angle of one of the normals
	r[0].set(cos(DEG_TO_RAD * angle), sin(DEG_TO_RAD * angle));
	//Rotate the other by 90 degrees to get the second normal
	r[1].set(-r[0].getY(), r[0].getX());
	//Get the vector for the slope
	Vec2D n = r[1].multiply(height).subtract(r[0].multiply(width)).unit();
	r[2].set(n.getY(), -n.getX());
	return r;
}


// Gets the vertices of the bounding box
Vec2D* Slope::getVertices(int* numVertices) {
	*numVertices = 3;
	Vec2D* r = new Vec2D[3];
	//Get vector in 1 direction at angle
	//Transpose it to get other vector
	Vec2D w = Vec2D(cos(DEG_TO_RAD * angle), sin(DEG_TO_RAD * angle));
	Vec2D h = Vec2D(-w.getY(), w.getX());
	//Scale to represent size of bounding box
	w.multiplyBy(width * 0.5);
	h.multiplyBy(height * 0.5);
	//Use vectors to get 3 corners
	r[0] = pos.subtract(w).add(h);
	r[1] = pos.subtract(w).subtract(h);
	r[2] = pos.add(w).subtract(h);
	return r;
}

