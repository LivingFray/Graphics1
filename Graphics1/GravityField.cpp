#include "GravityField.h"
#include "Globals.h"


GravityField::GravityField() {
	id = "gravfield";
	pos = Vec2D(0, 0);
	width = SMALLEST_THICKNESS;
	height = SMALLEST_THICKNESS;
	angle = 0;
	strength = 0;
	p = ParticleSystem();
	p.setEmitting(true);
	p.addColor(0, 127, 255, 127, 0, 127, 255, 127);
	p.addColor(0, 0, 255, 127, 0, 0, 255, 127);
	p.setTexture("Resources\\particles\\gravity.png");
	p.setMinSize(0.125);
	p.setMaxSize(0.125);
	p.setParticlesPerSecond(50);
	warmed = false;
}


GravityField::~GravityField() {
}


// Draws the gravity field ex seconds after last update
void GravityField::draw(double ex) {
	double w = width / 2;
	double h = height / 2;
	glPushMatrix();
	glTranslated(pos.getX(), pos.getY(), 0.0);
	//0 degrees should be down, rather than right
	glRotated(angle, 0, 0, 1);
	glBegin(GL_QUADS);
	glColor4ub(0, 0, 255, 20);
	glVertex2d(-1 * w, -1 * h);
	glVertex2d(w, -1 * h);
	glVertex2d(w, h);
	glVertex2d(-1 * w, h);
	glEnd();
	//Draw particles
	p.setPosition(Vec2D(0, height / 2));
	p.setAngle(-90);
	p.setMinAngle(-90);
	p.setMaxAngle(-90);
	p.setWidth(0);
	p.setHeight(width - 0.125);
	p.setMinVelocity(strength * 0.125);
	p.setMaxVelocity(strength * 0.175);
	p.setMinLife(height / (strength * 0.175));
	p.setMaxLife(height / (strength * 0.175));
	if (!warmed) {
		p.preWarm(1);
		warmed = true;
	}
	time += ex;
	p.draw(time - lastTime);
	lastTime = time;
	time -= ex;
	glPopMatrix();
}


// Updates the grabity field
void GravityField::update() {
	time += TICKRATE;
}


// Gets the strength of the gravity field
double GravityField::getStrength() {
	return strength;
}


// Sets the strength of the gravity field
void GravityField::setStrength(double strength) {
	this->strength = strength;
}


// Returns a DataObject representing the storable object
DataObject GravityField::save() {
	DataObject field = Storable::save();
	field.add("strength", strength);
	return field;
}


// Loads the storable object from the DataObject
void GravityField::load(DataObject obj) {
	Storable::load(obj);
	strength = obj.getDouble("strength");
}


// Sets the options for this selectable
void GravityField::setOptions(OptionMenu* menu) {
	Selectable::setOptions(menu);
	std::map<string, string> values = menu->getValues();
	string v;
	v = values["Strength"];
	setStrength(atof(v.c_str()));
}


// Creates an option menu using the current values as defaults
void GravityField::createOptions() {
	Selectable::createOptions();
	options->addOption("Strength", true, to_string(strength));
}