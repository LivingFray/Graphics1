#pragma once
#include <string>
#include <map>

enum DATATYPE { DATAEND, DATAOBJECT, BYTE, SHORT, INT, LONG, FLOAT, DOUBLE, STRING, UNKNOWN };

using namespace std;

class DataObject {
public:
	DataObject();
	~DataObject();
	// Adds an object to the dataobject to be saved
	void add(string key, DATATYPE type, void* data);
	// Returns the data object with matching key if it exists
	void* get(string key, DATATYPE &type);
	// Saves the data object to the file specified
	void saveToFile(string filePath);
	// Loads data into the object from the file specified
	void loadFromFile(string filePath);
private:
	void saveObject(string filePath);
	int loadObject(char* data, int length);
	void cpy(char* destination, const char* source, int sourceLength);
	struct Datum {
		DATATYPE type;
		void* data;
	};
	map<string, Datum*> dataList;
};

