#include "DataObject.h"
#include <iostream>
#include <fstream>

DataObject::DataObject() {
}


DataObject::DataObject(const DataObject& other) {
	for (auto it : other.dataList) {
		Datum* d = new Datum();
		d->type = it.second->type;
		switch (d->type) {
		case CHAR:
		{
			char data = *(char*)it.second->data;
			d->data = new char(data);
			break;
		}
		case SHORT:
		{
			short data = *(short*)it.second->data;
			d->data = new short(data);
			break;
		}
		case INT:
		{
			int data = *(int*)it.second->data;
			d->data = new int(data);
			break;
		}
		case LONG:
		{
			long data = *(long*)it.second->data;
			d->data = new long(data);
			break;
		}
		case FLOAT:
		{
			float data = *(float*)it.second->data;
			d->data = new float(data);
			break;
		}
		case DOUBLE:
		{
			double data = *(double*)it.second->data;
			d->data = new double(data);
			break;
		}
		case STRING:
		{
			std::string data = *(std::string*)it.second->data;
			d->data = new std::string(data);
			break;
		}
		case DATAOBJECT:
		{
			DataObject data = *(DataObject*)it.second->data;
			d->data = new DataObject(data);
			break;
		}
		}
		dataList[it.first] = d;
	}
}

DataObject DataObject::operator=(const DataObject& other) {
	if (this != &other) {
		for (auto it : other.dataList) {
			Datum* d = new Datum();
			d->type = it.second->type;
			switch (d->type) {
			case CHAR:
			{
				char data = *(char*)it.second->data;
				d->data = new char(data);
				break;
			}
			case SHORT:
			{
				short data = *(short*)it.second->data;
				d->data = new short(data);
				break;
			}
			case INT:
			{
				int data = *(int*)it.second->data;
				d->data = new int(data);
				break;
			}
			case LONG:
			{
				long data = *(long*)it.second->data;
				d->data = new long(data);
				break;
			}
			case FLOAT:
			{
				float data = *(float*)it.second->data;
				d->data = new float(data);
				break;
			}
			case DOUBLE:
			{
				double data = *(double*)it.second->data;
				d->data = new double(data);
				break;
			}
			case STRING:
			{
				std::string data = *(std::string*)it.second->data;
				d->data = new std::string(data);
				break;
			}
			case DATAOBJECT:
			{
				DataObject data = *(DataObject*)it.second->data;
				d->data = new DataObject(data);
				break;
			}
			}
			dataList[it.first] = d;
		}
	}
	return *this;
}

DataObject::~DataObject() {
	for (auto it : dataList) {
		deleteDatum(it.second);
	}
}

// Properly deletes a Datum struct
void DataObject::deleteDatum(Datum* d) {
	//Somehow datum was already gone
	if (!d) {
		return;
	}
	//I'm not sure if it would be safe to delete classes uncasted
	//As they are pointed to by void*
	if (d->type == UNKNOWN || d->type == DATAEND) {
		std::cerr << "Bad datatype found, attempting to delete" << std::endl;
	}
	switch (d->type) {
	case DATAOBJECT:
		delete (DataObject*)d->data;
		break;
	case STRING:
		delete (std::string*)d->data;
		break;
	default:
		delete d->data;
	}
	delete d;
}



void DataObject::saveObject(std::string filePath) {
	std::ofstream file;
	file.open(filePath, std::ios::binary | std::ios::out | std::ios::app);
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
		std::fill(buffer, (char*)buffer + bufferSize - 1, 0);
		//Save the data type (1 byte)
		buffer[0] = (char)d.second->type;
		//Save the length of the key (2 byte)
		uint16_t keyLen = (uint16_t)d.first.length();
		char* keyBin = new char[2];
		std::fill(keyBin, (char*)keyBin + 1, 0);
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
		case CHAR:
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
		} else if (d.second->type == STRING) {
			//If the data is a string save the length and data
			std::string* s = (std::string*)d.second->data;
			uint16_t strLen = (uint16_t)s->length();
			//Write the length of the string first
			buffer = (char*)&strLen;
			file.write(buffer, 2);
			//Write the actual characters of the string
			bufferSize = strLen;
			if (bufferSize > 0) {
				buffer = new char[bufferSize];
				//Clear the array to prevent any junk being written
				std::fill(buffer, (char*)buffer + bufferSize - 1, '\0');
				//Put the string into the buffer then save the buffer
				cpy((char*)buffer, s->data(), strLen);
				file.write(buffer, bufferSize);
				delete[] buffer;
			}
		} else if (d.second->type == DATAOBJECT) {
			//A bit hacky, but recursively save the object
			DataObject* subObj = (DataObject*)d.second->data;
			file.flush();
			file.close();
			subObj->saveObject(filePath);
			file.open(filePath, std::ios::binary | std::ios::out | std::ios::app);
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
		DataType type = (DataType)data[pos];
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
		std::string key = std::string(keyChar);
		pos += *keyLen;
		delete[] keyChar;
		//Act on the datatype accordingly
		int normalType = 0;
		switch (type) {
		case CHAR:
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
			Datum* item = new Datum();
			item->type = type;
			item->data = newData;
			if (dataList[key]) {
				deleteDatum(dataList[key]);
			}
			dataList[key] = item;
			pos += normalType;
		} else if (type == STRING) {
			//Read the length of the string in
			uint16_t* strLen = (uint16_t*)(data + pos);
			pos += 2;
			//Create a char array to hold the string
			char* strChar = new char[*strLen + 1];
			cpy(strChar, (char*)data + pos, *strLen);
			//Make the string null terminated
			strChar[*strLen] = '\0';
			//Save the string
			add(key, std::string(strChar));
			delete[] strChar;
			pos += *strLen;
		} else if (type == DATAOBJECT) {
			//Recurvisely load the sub objects
			DataObject subObj = DataObject();
			pos += subObj.loadObject((char*)data + pos, length - pos);
			add(key, subObj);
		}
	}
	return pos;
}


void DataObject::cpy(char* destination, const char* source, int sourceLength) {
	for (int i = 0; i < sourceLength; i++) {
		destination[i] = source[i];
	}
}


// Attempts to load a DataObject from the path given
void DataObject::loadFromFile(std::string filePath) {
	std::ifstream file;
	file.open(filePath, std::ios::binary | std::ios::in);
	if (!file.is_open()) {
		std::cerr << "Could not open file for reading (may not exist)" << std::endl;
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
		std::cerr << "Could not read all of file" << std::endl;
		delete raw;
		return;
	}
	file.close();
	//Attempt to parse the input file
	int l = loadObject(raw, length);
	if (l != length) {
		std::cerr << "Didn't parse entire file (" << l << "/" << length << ")" << std::endl;
	}
	//Cleanup the memory used to hold the file
	delete[] raw;
}


// Saves the DataObject to the specified file
void DataObject::saveToFile(std::string filePath) {
	std::ofstream file;
	//First clear the file should it already exist
	file.open(filePath, std::ios::binary | std::ios::out);
	file.write("", 0);
	file.flush();
	file.close();
	//Iterate through the objects and save them
	saveObject(filePath);
}


// Adds a character to the data object under the specified key
void DataObject::add(std::string key, char data) {
	Datum* d = new Datum();
	d->type = CHAR;
	d->data = new char(data);
	if (dataList[key]) {
		deleteDatum(dataList[key]);
	}
	dataList[key] = d;
}


// Adds a short to the data object under the specified key
void DataObject::add(std::string key, short data) {
	Datum* d = new Datum();
	d->type = SHORT;
	d->data = new short(data);
	if (dataList[key]) {
		deleteDatum(dataList[key]);
	}
	dataList[key] = d;
}


// Adds an int to the data object under the specified key
void DataObject::add(std::string key, int data) {
	Datum* d = new Datum();
	d->type = INT;
	d->data = new int(data);
	if (dataList[key]) {
		deleteDatum(dataList[key]);
	}
	dataList[key] = d;
}


// Adds a long to the data object under the specified key
void DataObject::add(std::string key, long data) {
	Datum* d = new Datum();
	d->type = LONG;
	d->data = new long(data);
	if (dataList[key]) {
		deleteDatum(dataList[key]);
	}
	dataList[key] = d;
}


// Adds a float to the data object under the specified key
void DataObject::add(std::string key, float data) {
	Datum* d = new Datum();
	d->type = FLOAT;
	d->data = new float(data);
	if (dataList[key]) {
		deleteDatum(dataList[key]);
	}
	dataList[key] = d;
}


// Adds a double to the data object under the specified key
void DataObject::add(std::string key, double data) {
	Datum* d = new Datum();
	d->type = DOUBLE;
	d->data = new double(data);
	if (dataList[key]) {
		deleteDatum(dataList[key]);
	}
	dataList[key] = d;
}


// Adds a string to the data object under the specified key
void DataObject::add(std::string key, std::string data) {
	Datum* d = new Datum();
	d->type = STRING;
	d->data = new std::string(data);
	if (dataList[key]) {
		deleteDatum(dataList[key]);
	}
	dataList[key] = d;
}


// Adds a dataobject to the data object under the specified key
void DataObject::add(std::string key, DataObject data) {
	Datum* d = new Datum();
	d->type = DATAOBJECT;
	d->data = new DataObject(data);
	if (dataList[key]) {
		deleteDatum(dataList[key]);
	}
	dataList[key] = d;
}


// Gets the character stored under the specified key
char DataObject::getChar(std::string key) {
	bool e;
	return getChar(key, e);
}


// Gets the character stored under the specified key
short DataObject::getShort(std::string key) {
	bool e;
	return getShort(key, e);
}


// Gets the character stored under the specified key
int DataObject::getInt(std::string key) {
	bool e;
	return getInt(key, e);
}


// Gets the character stored under the specified key
long DataObject::getLong(std::string key) {
	bool e;
	return getLong(key, e);
}


// Gets the character stored under the specified key
float DataObject::getFloat(std::string key) {
	bool e;
	return getFloat(key, e);
}


// Gets the character stored under the specified key
double DataObject::getDouble(std::string key) {
	bool e;
	return getDouble(key, e);
}


// Gets the character stored under the specified key
std::string DataObject::getString(std::string key) {
	bool e;
	return getString(key, e);
}


// Gets the character stored under the specified key
DataObject DataObject::getDataObject(std::string key) {
	bool e;
	return getDataObject(key, e);
}


// Gets the character stored under the specified key
char DataObject::getChar(std::string key, bool &exists) {
	if (!dataList[key] || dataList[key]->type != CHAR) {
		exists = false;
		return NULL;
	}
	exists = true;
	return *(char*)dataList[key]->data;
}


// Gets the character stored under the specified key
short DataObject::getShort(std::string key, bool &exists) {
	if (!dataList[key] || dataList[key]->type != SHORT) {
		exists = false;
		return NULL;
	}
	exists = true;
	return *(short*)dataList[key]->data;
}


// Gets the character stored under the specified key
int DataObject::getInt(std::string key, bool &exists) {
	if (!dataList[key] || dataList[key]->type != INT) {
		exists = false;
		return NULL;
	}
	exists = true;
	return *(int*)dataList[key]->data;
}


// Gets the character stored under the specified key
long DataObject::getLong(std::string key, bool &exists) {
	if (!dataList[key] || dataList[key]->type != LONG) {
		exists = false;
		return NULL;
	}
	exists = true;
	return *(long*)dataList[key]->data;
}


// Gets the character stored under the specified key
float DataObject::getFloat(std::string key, bool &exists) {
	if (!dataList[key] || dataList[key]->type != FLOAT) {
		exists = false;
		return NULL;
	}
	exists = true;
	return *(float*)dataList[key]->data;
}


// Gets the character stored under the specified key
double DataObject::getDouble(std::string key, bool &exists) {
	if (!dataList[key] || dataList[key]->type != DOUBLE) {
		exists = false;
		return NULL;
	}
	exists = true;
	return *(double*)dataList[key]->data;
}


// Gets the character stored under the specified key
std::string DataObject::getString(std::string key, bool &exists) {
	if (!dataList[key] || dataList[key]->type != STRING) {
		exists = false;
		return std::string();
	}
	exists = true;
	return *(std::string*)dataList[key]->data;
}


// Gets the character stored under the specified key
DataObject DataObject::getDataObject(std::string key, bool &exists) {
	if (!dataList[key] || dataList[key]->type != DATAOBJECT) {
		exists = false;
		return DataObject();
	}
	exists = true;
	return *(DataObject*)dataList[key]->data;
}
