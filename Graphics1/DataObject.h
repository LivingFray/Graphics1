#pragma once
#include <string>
#include <map>

enum DataType {DATAEND, DATAOBJECT, CHAR, SHORT, INT, LONG, FLOAT, DOUBLE, STRING, UNKNOWN};

class DataObject {
public:
	DataObject();
	DataObject(const DataObject& other);
	~DataObject();
	DataObject operator=(const DataObject& other);
	// Attempts to load a DataObject from the path given
	void loadFromFile(std::string filePath);
	// Saves the DataObject to the specified file
	void saveToFile(std::string filePath);
	// Adds a character to the data object under the specified key
	void add(std::string key, char data);
	// Adds a short to the data object under the specified key
	void add(std::string key, short data);
	// Adds an int to the data object under the specified key
	void add(std::string key, int data);
	// Adds a long to the data object under the specified key
	void add(std::string key, long data);
	// Adds a float to the data object under the specified key
	void add(std::string key, float data);
	// Adds a double to the data object under the specified key
	void add(std::string key, double data);
	// Adds a string to the data object under the specified key
	void add(std::string key, std::string data);
	// Adds a dataobject to the data object under the specified key
	void add(std::string key, DataObject data);
	// Gets the character stored under the specified key
	char getChar(std::string key, bool &exists);
	// Gets the short stored under the specified key
	short getShort(std::string key, bool &exists);
	// Gets the int stored under the specified key
	int getInt(std::string key, bool &exists);
	// Gets the long stored under the specified key
	long getLong(std::string key, bool &exists);
	// Gets the float stored under the specified key
	float getFloat(std::string key, bool &exists);
	// Gets the double stored under the specified key
	double getDouble(std::string key, bool &exists);
	// Gets the string stored under the specified key
	std::string getString(std::string key, bool &exists);
	// Gets the dataobject stored under the specified key
	DataObject getDataObject(std::string key, bool &exists);
	// Gets the character stored under the specified key
	char getChar(std::string key);
	// Gets the short stored under the specified key
	short getShort(std::string key);
	// Gets the int stored under the specified key
	int getInt(std::string key);
	// Gets the long stored under the specified key
	long getLong(std::string key);
	// Gets the float stored under the specified key
	float getFloat(std::string key);
	// Gets the double stored under the specified key
	double getDouble(std::string key);
	// Gets the string stored under the specified key
	std::string getString(std::string key);
	// Gets the dataobject stored under the specified key
	DataObject getDataObject(std::string key);
private:
	struct Datum {
		DataType type;
		void* data;
	};
	std::map<std::string, Datum*> dataList;
	void deleteDatum(Datum* d);
	void saveObject(std::string filePath);
	int loadObject(char* data, int length);
	void cpy(char* destination, const char* source, int sourceLength);
};

