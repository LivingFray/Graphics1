#pragma once
class Vec2D {
public:
	Vec2D();
	Vec2D(double x, double y);
	~Vec2D();
protected:
	double x;
	double y;
public:
	// Gets the x component of the vector
	double getX();
	// Gtes the y component of the vector
	double getY();
	// Sets the x component of the vector
	void setX(double x);
	// Sets the y component of the vector
	void setY(double y);
	// Sets both components of the vector
	void set(double x, double y);
	// Adds the given vector to the original
	void addTo(Vec2D* other);
	// Subtracts the given vector from the original
	void subtractFrom(Vec2D* other);
	// Multiplies the vector by the value given
	void multiplyBy(double scale);
	// Divides the vector by the value given
	void divideBy(double scale);
	// Calculates the dot product of the 2 vectors
	double dot(Vec2D* other);
	// Converts the vector to unit length
	void toUnit();
	// Gets the unit vector for this vector
	Vec2D* unit();
	// Returns a vector equal to the sum of the two vectors
	Vec2D* add(Vec2D* other);
	// Returns a vector equal to the difference of the two vectors
	Vec2D* subtract(Vec2D* other);
	// Returns the magnitude of the vector
	double magnitude();
	// Returns the square of the magnitude of the vector
	double magnitudeSquare();
	// Returns a vector multiplied by scale
	Vec2D* multiply(double scale);
	// Returns a vector divided by scale
	Vec2D* divide(double scale);
};

