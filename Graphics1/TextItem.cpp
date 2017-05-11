#include "TextItem.h"
#include "Globals.h"
#include "Level.h"

TextItem::TextItem() {
	id = "text";
	a = 255;
	width = 0.25;
	height = 0.25;
}


TextItem::~TextItem() {
}


// Sets the text to be displayed
void TextItem::setText(string text) {
	this->text = text;
}


// Gets the text to be displayed
string TextItem::getText() {
	return text;
}


// Sets whether to use the large or small font
void TextItem::setLargeFontUsed(bool useLarge) {
	this->largeFont = useLarge;
}


// Gets the font size used
bool TextItem::getLargeFontUsed() {
	return largeFont;
}


// Sets the colour of the text
void TextItem::setColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}


// Gets the Red Colour of the text
unsigned char TextItem::getRed() {
	return r;
}


// Gets the Green Colour of the text
unsigned char TextItem::getGreen() {
	return g;
}


// Gets the Blue Colour of the text
unsigned char TextItem::getBlue() {
	return b;
}


// Gets the Alpha Colour of the text
unsigned char TextItem::getAlpha() {
	return a;
}


// Draws the text ex seconds after last update
void TextItem::draw(double ex) {
	if (editorMode) {
		glPushMatrix();
		glTranslated(pos.getX(), pos.getY(), 0);
		glRotated(angle, 0, 0, 1);
		glColor3ub(0, 0, 0);
		glLineWidth(2);
		glBegin(GL_LINE_LOOP);
		glVertex2d(-width / 2, -height / 2);
		glVertex2d(-width / 2, height / 2);
		glVertex2d(width / 2, height / 2);
		glVertex2d(width / 2, -height / 2);
		glEnd();
		glPopMatrix();
	}
	glPushMatrix();
	glLoadIdentity();
	Level* l = (Level*)state;
	Vec2D offset;
	freetype::font_data f = largeFont ? fontLarge : fontSmall;
	int len = freetype::getLength(f, text.c_str());
	double h = f.h * 0.625;
	offset.setX(-len / 2.0);
	offset.setY(-h);
	offset.multiplyBy(WORLD_SIZE / (double)(sWidth < sHeight ? sWidth : sHeight));
	//Add extrapolation
	Player* p = l->getPlayer();
	if (!editorMode && p) {
		offset.subtractFrom(p->getVel().multiply(ex));
	}
	Vec2D screen = l->getScreenCoordinates(pos.add(offset));
	glTranslated(len / 2 + screen.getX(), h + screen.getY(), 0);
	glRotated(angle - l->getCameraAngleAt(0), 0, 0, 1);
	glTranslated(-len / 2.0, -h, 0);
	glColor4ub(r, g, b, a);
	freetype::print(f, 0, 0, text.c_str());
	glPopMatrix();
}


// Returns a DataObject representing the storable object
DataObject TextItem::save() {
	DataObject platform = Storable::save();
	platform.add("text", text);
	platform.add("r", (char)r);
	platform.add("g", (char)g);
	platform.add("b", (char)b);
	platform.add("a", (char)a);
	return platform;
}


// Loads the storable object from the DataObject
void TextItem::load(DataObject obj) {
	Storable::load(obj);
	setText(obj.getString("text"));
	r = (unsigned char)obj.getChar("r");
	g = (unsigned char)obj.getChar("g");
	b = (unsigned char)obj.getChar("b");
	a = (unsigned char)obj.getChar("a");
}


// Sets the options for this selectable
void TextItem::setOptions(OptionMenu* menu) {
	Selectable::setOptions(menu);
	std::map<string, string> values = menu->getValues();
	string v;
	v = values["Text"];
	text = v;
	v = values["Red"];
	r = ((unsigned char)atoi(v.c_str()));
	v = values["Green"];
	g = ((unsigned char)atoi(v.c_str()));
	v = values["Blue"];
	b = ((unsigned char)atoi(v.c_str()));
	v = values["Alpha"];
	a = ((unsigned char)atoi(v.c_str()));
}


// Creates an option menu using the current values as defaults
void TextItem::createOptions() {
	Selectable::createOptions();
	options->addOption("Text", false, text);
	options->addOption("Red", true, to_string(r));
	options->addOption("Green", true, to_string(g));
	options->addOption("Blue", true, to_string(b));
	options->addOption("Alpha", true, to_string(a));
}


// Returns whether the object is solid
bool TextItem::isSolid() {
	return false;
}


// Returns if the world object can be rotated
bool TextItem::canResize() {
	return false;
}
