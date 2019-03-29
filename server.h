#pragma once
#include <string>
#include <vector>
#include "config.h"
using namespace std;

// TODO: Currently all class members are in public, will sort that out later

class bytevector {
public:
	char *body;
	int size;
	bytevector(int n = 0);
	bytevector(const char *, int); // TODO: what if length of const char * is less than given int?
	bytevector(string const&);
	// TODO: other constructors?
	~bytevector();
	
	char operator[](int) const;
	char & operator[](int);
	int write_to_file(string const &) const; // TODO: generalize (do not want to write in Audios directory)
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

class DataSamples : public SObject {
public:
	int N; // number of points on a 2D plane
	double *y; // pointer to array with values
	// x values can be uniformly distributed (as in most cases), so we do not save it here
	
	DataSamples(int n = 0);
	DataSamples(const double *, int); // TODO: what if size of const double * is less than int?
	DataSamples(vector<double> const &);
	
	virtual ~DataSamples() {};
	virtual int getObjId() const = 0; // identificator of a class
	virtual int init(bytevector const &) = 0; // initialize a copied prototype object with bytevector values
	virtual bytevector serialize() const = 0; // convert class object to bytevector
	virtual DataSamples * clone() const = 0; // copy a prototype
	virtual void print() const = 0;
};

class UniformDataSamples : public DataSamples {
public:
	double x_0; // most left point
	double delta_x; // interval between x coordinates
	
	UniformDataSamples(int n = 0);
	UniformDataSamples(const double *, int, double, double); // TODO: what if size of const double * is less than int?
	UniformDataSamples(vector<double> const &, double, double);
	UniformDataSamples(UniformDataSamples const &);
	UniformDataSamples &operator=(UniformDataSamples const &);
	
	double &operator[](int);
	double operator[](int) const; 
		
	~UniformDataSamples();
	int getObjId() const; // identificator of a class
	int init(bytevector const &); // initialize a copied prototype object with bytevector values
	bytevector serialize() const; // convert class object to bytevector
	UniformDataSamples * clone() const; // copy a prototype
	void print() const;
};


class AudioFile : public SObject { 
public:
	string file_id; // name of a file, it's unique id (that is synced with client part)
	int size; // file size in bytes
	int fd; // file descriptor so we can close the file later
	
	int *data; // pointer to mmap object with the whole file
	int NumSamples; // number of points in data
	int SampleRate; // number of points per second

	AudioFile(string file_id, int size, int fd, int *data, int NumSamples, int SampleRate);
	virtual ~AudioFile() {};
	virtual int getObjId() const = 0; 
	virtual int init(bytevector const &) = 0;
	virtual bytevector serialize() const = 0;
	virtual AudioFile * clone() const = 0;
	
	virtual void print() const = 0;
	
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
	~WAV_File(); 
	int getObjId() const; // returns a code that it is a WAV file
	int init(bytevector const &); // parsing "WAVparse.cc"
	bytevector serialize() const; // compressing to a file of WAV format
	WAV_File * clone() const; // for Object Factory
	
	UniformDataSamples getSamples() const; // returns a container with data points
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
	// TODO: add more specifications
};


