#include "DataObject.h"
#include <iostream>
#include <fstream>
using namespace std;

DataObject::DataObject() {
}


DataObject::~DataObject() {
	for (auto const& d : dataList) {
		delete d.second;
	}
}


// Adds an object to the dataobject to be saved
void DataObject::add(string key, DATATYPE type, void* data) {
	Datum* datum = new Datum();
	datum->type = type;
	datum->data = data;
	dataList[key] = datum;
}


// Returns the data object with matching key if it exists
void* DataObject::get(string key, DATATYPE &type) {

	Datum* datum = dataList[key];
	if (datum) {
		type = datum->type;
	}
	else {
		//If the requested item is non existant, return null
		type = UNKNOWN;
		return NULL;
	}
	return datum->data;
}

void DataObject::saveObject(string filePath) {
	ofstream file;
	file.open(filePath, ios::binary | ios::out | ios::app);
	//The size of the data to next be written to file
	int bufferSize;
	//A buffer to put data in that is to be saved
	char* buffer;
	//For each datum in the dataobject, write it to file
	for (auto const& d : dataList) {
		if (d.second->type == UNKNOWN) {
			continue; //Someone messed up and I'm not saving their mess
		}
		/*
		 * First write the header for the data
		 * Header contains 1 byte defining the type, 2 bytes specifying the key
		 * length and [length] bytes containing the key
		 */
		 //Preallocate a buffer to put the header to write in
		bufferSize = 1 + 2 + d.first.length();
		buffer = new char[bufferSize];
		fill(buffer, (char*)buffer + bufferSize - 1, 0);
		//Save the data type (1 byte)
		buffer[0] = (char)d.second->type;
		//Save the length of the key (2 byte)
		uint16_t keyLen = (uint16_t)d.first.length();
		char* keyBin = new char[2];
		fill(keyBin, (char*)keyBin + 1, 0);
		keyBin = (char*)&keyLen;
		strncpy_s((char*)buffer + 1, bufferSize - 1, keyBin, 2);
		//Save the key (length bytes)
		int l = d.first.length();
		char* b = (char*)buffer + 3;
		cpy(b, d.first.data(), d.first.length());
		//Write the data to file
		file.write(buffer, bufferSize);
		delete[] buffer;
		/*
		 * Next write the content for the data, the size of which is dependent on
		 * the data type
		 */
		int normalType = 0; //Acts as a boolean to determine if I can dump
							//The binary without processing it, and holds
							//The size of the data
		switch (d.second->type) {
		case BYTE:
			normalType = 1;
			break;
		case SHORT:
			normalType = 2;
			break;
		case INT:
			normalType = 4;
			break;
		case LONG:
			normalType = 8;
			break;
		case FLOAT:
			normalType = 4;
			break;
		case DOUBLE:
			normalType = 8;
			break;
		}
		if (normalType) {
			//If the data can simply be parsed to char* then do so
			buffer = (char*)d.second->data;
			file.write(buffer, normalType);
		}
		else if (d.second->type == STRING) {
			//If the data is a string save the length and data
			string* s = (string*)d.second->data;
			uint16_t strLen = (uint16_t)s->length();
			//Write the length of the string first
			buffer = (char*)&strLen;
			file.write(buffer, 2);
			//Write the actual characters of the string
			bufferSize = strLen;
			buffer = new char[bufferSize];
			//Clear the array to prevent any junk being written
			fill(buffer, (char*)buffer + bufferSize - 1, '\0');
			//Put the string into the buffer then save the buffer
			cpy((char*)buffer, s->data(), strLen);
			file.write(buffer, bufferSize);
			delete[] buffer;
		}
		else if (d.second->type == DATAOBJECT) {
			//A bit hacky, but recursively save the object
			DataObject* subObj = (DataObject*)d.second->data;
			file.flush();
			file.close();
			subObj->saveObject(filePath);
			file.open(filePath, ios::binary | ios::out | ios::app);
			//Write the end of the dataobject
			bufferSize = 1;
			buffer = new char[bufferSize];
			buffer[0] = DATAEND;
			file.write(buffer, bufferSize);
			delete[] buffer;
		}
	}
	file.flush();
	file.close();
}

int DataObject::loadObject(char* data, int length) {
	//How far through the file has been parsed
	int pos = 0;
	//Loop through so long as there is another header to read
	while (pos + 3 < length) {
		//Read the datatype
		DATATYPE type = (DATATYPE)data[pos];
		pos++;
		//DataEnd is a special case where the header has no key
		if (type == DATAEND) {
			return pos;
		}
		//Read the key length
		uint16_t* keyLen = (uint16_t*)(data + pos);
		pos += 2;
		//Read the key
		char* keyChar = new char[*keyLen + 1];
		cpy(keyChar, (char*)data + pos, *keyLen);
		//Make the string null terminating
		keyChar[*keyLen] = '\0';
		string key = string(keyChar);
		pos += *keyLen;
		delete[] keyChar;
		//Act on the datatype accordingly
		int normalType = 0;
		switch (type) {
		case BYTE:
			normalType = 1;
			break;
		case SHORT:
			normalType = 2;
			break;
		case INT:
			normalType = 4;
			break;
		case LONG:
			normalType = 8;
			break;
		case FLOAT:
			normalType = 4;
			break;
		case DOUBLE:
			normalType = 8;
			break;
		}
		if (normalType) {
			//Similar to in save, the data can be directly converted
			//Prevent data being purged later
			char* newData = new char[normalType];
			cpy(newData, data + pos, normalType);
			//Add the data to this dataobject
			add(key, type, newData);
			pos += normalType;
		}
		else if (type == STRING) {
			//Read the length of the string in
			uint16_t* strLen = (uint16_t*)(data + pos);
			pos += 2;
			//Create a char array to hold the string
			char* strChar = new char[*strLen + 1];
			cpy(strChar, (char*)data + pos, *strLen);
			//Make the string null terminated
			strChar[*strLen] = '\0';
			//Convert from a c-string to a c++ string
			string* strStr = new string(strChar);
			delete[] strChar;
			//Save the string
			add(key, STRING, strStr);
			pos += *strLen;
		}
		else if (type == DATAOBJECT) {
			//Recurvisely load the sub objects
			DataObject* subObj = new DataObject();
			pos += subObj->loadObject((char*)data + pos, length - pos);
			add(key, DATAOBJECT, subObj);
		}
	}
	return pos;
}

void DataObject::cpy(char* destination, const char* source, int sourceLength) {
	for (int i = 0; i < sourceLength; i++) {
		destination[i] = source[i];
	}
}


// Saves the data object to the file specified
void DataObject::saveToFile(string filePath) {
	ofstream file;
	//First clear the file should it already exist
	file.open(filePath, ios::binary | ios::out);
	file.write("", 0);
	file.flush();
	file.close();
	//Iterate through the objects and save them
	saveObject(filePath);
}

// Loads data into the object from the file specified
void DataObject::loadFromFile(string filePath) {
	ifstream file;
	file.open(filePath, ios::binary | ios::in);
	if (!file.is_open()) {
		cerr << "Could not open file for reading (may not exist)" << endl;
		file.close();
		return;
	}
	//Get file length
	file.seekg(0, file.end);
	int length = (int)file.tellg();
	file.seekg(0, file.beg);
	//Create a block of memory to hold the file in
	char* raw = new char[length];
	file.read(raw, length);
	if (!file) {
		cerr << "Could not read all of file" << endl;
		file.close();
		delete[] raw;
		return;
	}
	file.close();
	//Attempt to parse the input file
	int l = loadObject(raw, length);
	if (l != length) {
		cerr << "Didn't parse entire file (" << l << "/" << length << ")" << endl;
	}
	//Cleanup the memory used to hold the file
	delete[] raw;
}
