#pragma once
#include <string>
#include "config.h"
using namespace std;

// Currently all class members are in public, will sort that out later

class bytevector {
public:
	char *body;
	int size;
	bytevector();
	bytevector(const char *, int);
	bytevector(string const&);
	// other constructors?
	~bytevector();
	// receiving bytes in chunks? depends on buffer size
	
	int write_to_file(string const &) const;
	int read_from_file(string const &);
	void print() const;
};

class SObject { // Serializable Object
public:
	virtual int getObjId() const = 0; // identificator of a class
	virtual int init(bytevector const &) = 0; // initialize a copied prototype object with bytevector values
	virtual bytevector serialize() const = 0; // convert class object to bytevector
	virtual SObject * clone() const = 0; // copy a prototype
};

class AudioFile : public SObject { 
public:
	string file_id; // name of a file, it's unique id (that is synced with client part)
	int size; // file size in bytes
	int fd; // file descriptor so we can close the file later
	
	int *data; // pointer to mmap object with the whole file (or should it point to a heap array?)
	int NumSamples; // number of points in data
	int SampleRate; // number of points per second

	AudioFile(string file_id, int size, int fd, int *data, int NumSamples, int SampleRate); // look at geometry.cc on March 1st
	//virtual ~AudioFile();
	virtual int getObjId() const = 0; 
	virtual int init(bytevector const &) = 0;
	virtual bytevector serialize() const = 0;
	virtual AudioFile * clone() const = 0;
};

class WAV_File : public AudioFile {
public:
	int Subchunk1Size; // 16 for PCM. This is the size of the rest of the Subchunk which follows this number
	int AudioFormat; // PCM = 1 (i.e. linear quantization)
	int NumChannels; // Mono - 1, Stereo - 2
	int ByteRate; // == SampleRate * NumChannels * BitsPerSample/8
	int BlockAlign; // == NumChannels * BitsPerSample/8
	int BitDepth; // a.k.a. # bits per sample
	
	WAV_File(); // default constructor for prototype object for Object Factory
	~WAV_File(); // from "WAVparse.cc"
	int getObjId() const; // returns a code that it is a WAV file
	int init(bytevector const &); // parsing "WAVparse.cc"
	bytevector serialize() const; // compressing to a file of WAV format
	WAV_File * clone() const; // for Object Factory
	
	void print() const;
};

class Query : public SObject {
private:
	string file_id; // 8 first bytes of a Query bytevector
public:
	virtual int getObjId() const = 0;
	virtual int init(bytevector const &) = 0;
	virtual bytevector serialize() const = 0;
	virtual Query * clone() const = 0;
};

class Crop : public Query {
public:
	int getObjId() const; // id of crop function
	int init(bytevector const &); // for Object Factory
	bytevector serialize() const; // convert to bytevector
	Query * clone() const; // for Object Factory
	// add more specifications
};