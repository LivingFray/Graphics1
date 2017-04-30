#pragma once
#include "Scenery.h"
class TextItem :
	public Scenery {
public:
	TextItem();
	~TextItem();
	// Sets the text to be displayed
	void setText(string text);
	// Gets the text to be displayed
	string getText();
	// Sets whether to use the large or small font
	void setLargeFontUsed(bool useLarge);
	// Gets the font size used
	bool getLargeFontUsed();
	// Sets the colour of the text
	void setColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	// Gets the Red Colour of the text
	unsigned char getRed();
	// Gets the Green Colour of the text
	unsigned char getGreen();
	// Gets the Blue Colour of the text
	unsigned char getBlue();
	// Gets the Alpha Colour of the text
	unsigned char getAlpha();
	// Draws the text ex seconds after last update
	void draw(double ex);
	// Returns a DataObject representing the storable object
	DataObject save();
	// Loads the storable object from the DataObject
	void load(DataObject obj);
	// Sets the options for this selectable
	void setOptions(OptionMenu * menu);
	// Creates an option menu using the current values as defaults
	void createOptions();
	// Returns whether the object is solid
	bool isSolid();
	// Returns if the world object can be resized
	bool canResize();
protected:
	string text;
	bool largeFont;
	unsigned char r, g, b, a;
};

