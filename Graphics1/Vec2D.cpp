#include "Vec2D.h"
#include <Math.h>


Vec2D::Vec2D() {
	x = 0.0;
	y = 0.0;
}

Vec2D::Vec2D(double x, double y) {
	this->x = x;
	this->y = y;
}

Vec2D::~Vec2D() {
}


// Gets the x component of the vector
double Vec2D::getX() {
	return x;
}


// Gtes the y component of the vector
double Vec2D::getY() {
	return y;
}


// Sets the x component of the vector
void Vec2D::setX(double x) {
	this->x = x;
}


// Sets the y component of the vector
void Vec2D::setY(double y) {
	this->y = y;
}


// Sets both components of the vector
void Vec2D::set(double x, double y) {
	this->x = x;
	this->y = y;
}



// Adds the given vecto to the original
void Vec2D::addTo(Vec2D other) {
	x += other.x;
	y += other.y;
}


// Subtracts the given vector from the original
void Vec2D::subtractFrom(Vec2D other) {
	x -= other.x;
	y -= other.y;
}


// Multiplies the vector by the value given
void Vec2D::multiplyBy(double scale) {
	x *= scale;
	y *= scale;
}

// Divides the vector by the value given
void Vec2D::divideBy(double scale) {
	x /= scale;
	y /= scale;
}


// Calculates the dot product of the 2 vectors
double Vec2D::dot(Vec2D other) {
	return x * other.x + y * other.y;
}


// Converts the vector to unit length
void Vec2D::toUnit() {
	double m = magnitude();
	divideBy(m);
}


// Gets the unit vector for this vector
Vec2D Vec2D::unit() {
	Vec2D r = Vec2D(x, y);
	r.toUnit();
	return r;
}


// Returns a vector equal to the sum of the two vectors
Vec2D Vec2D::add(Vec2D other) {
	Vec2D r = Vec2D(x, y);
	r.addTo(other);
	return r;
}


// Returns a vector equal to the difference of the two vectors
Vec2D Vec2D::subtract(Vec2D other) {
	Vec2D r = Vec2D(x, y);
	r.subtractFrom(other);
	return r;
}


// Returns the magnitude of the vector
double Vec2D::magnitude() {
	return sqrt(magnitudeSquare());
}


// Returns the square of the magnitude of the vector
double Vec2D::magnitudeSquare() {
	return x * x + y * y;
}


// Returns a vector multiplied by scale
Vec2D Vec2D::multiply(double scale) {
	Vec2D r = Vec2D(x, y);
	r.multiplyBy(scale);
	return r;
}


// Returns a vector divided by scale
Vec2D Vec2D::divide(double scale) {
	Vec2D r = Vec2D(x, y);
	r.divideBy(scale);
	return r;
}
