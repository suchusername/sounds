#pragma once
#include <string>
#include <vector>
#include "config.h"
using namespace std;

// TODO: Currently all class members are in public, will sort that out later

class bytevector {
private:
	long FdGetFileSize(int);
	
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
	int len() const;
	void print() const;
	void writeString(const string &, int);
	void writeInt(int, int);
};

class SObject { // Serializable Object
public:
	virtual int init(bytevector const &, const string &) = 0; // initialize a copied prototype object with bytevector values
	virtual bytevector serialize() const = 0; // convert class object to bytevector
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
	virtual int init(bytevector const &, const string &) = 0; // initialize a copied prototype object with bytevector values
	virtual bytevector serialize() const = 0; // convert class object to bytevector
	virtual void print() const = 0;
};

class UniformDataSamples : public DataSamples {
public:
	void writeInt(int, bytevector &, int) const;
	int readInt(bytevector const &, int) const;
	void writeDouble(double, bytevector &, int) const;
	double readDouble(bytevector const &, int) const;
	
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
	int init(bytevector const &, const string &s = ""); // initialize a copied prototype object with bytevector values
	bytevector serialize() const; // convert class object to bytevector
	
	void print() const;
	void crop(int, int, bool); // only leave the [l,r) part of vector
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
	virtual int load(string const &) = 0;
	virtual int init(bytevector const &, const string &) = 0;
	virtual bytevector serialize() const = 0;
	
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
	int offset; // offset of data due to possible extra data inside of a file
	
	WAV_File(); // default constructor for prototype object for Object Factory
	~WAV_File(); 
	int load(string const &);
	int init(bytevector const &, const string &fcode = DEFAULT_FCODE); // parsing "WAVparse.cc"
	bytevector serialize() const; // compressing to a file of WAV format
	
	UniformDataSamples getSamples() const; // returns a container with data points
	void print() const;
};


class Query : public SObject {
public:
	void writeUll(unsigned long long, bytevector &, int) const;
	unsigned long long readUll(bytevector const &, int) const;
	void writeInt(int, bytevector &, int) const;
	int readInt(bytevector const &, int) const;
	void writeShort(int, bytevector &, int) const;
	bool is_number(const string &) const;
	
public:
	string file_id; // name of a file to edit
	
	Query();
	Query(string);
	virtual int init(bytevector const &, const string &) = 0;
	virtual bytevector serialize() const = 0;
	
	virtual void print() const = 0;
	virtual void transform(WAV_File *, const string &) const = 0; // string - new id of a file
};

class Crop : public Query {
public:
	// Interval [left, right) in terms of samples is left
	// if right = -1, then right is max possible
	int left;
	int right;
	
	Crop();
	Crop(const string &, int l = 0, int r = -1);
	
	int init(bytevector const &, const string &s = "");
	bytevector serialize() const; // convert to bytevector
	
	void print() const;
	void transform(WAV_File *, const string &) const;
};

class Volume : public Query {
public:
	void writeInt(int, bytevector &, int) const;
	int readInt(bytevector const &, int) const;
	void writeDouble(double, bytevector &, int) const;
	double readDouble(bytevector const &, int) const;
	
public:
	/*
	Multiplies every value in [left, right) by k
	If value exceeds max = 32768 then it stays there
	
	If (smooth == true) then values are multiplied by a smooth function with peak =k.
	
	*/
	
	double k;
	int left;
	int right;
	bool smooth;
	
	Volume();
	Volume(const string &, double, int l = 0, int r = -1, bool smooth = false);
	
	int init(bytevector const &, const string &s = "");
	bytevector serialize() const;
	
	void print() const;
	void transform(WAV_File *, const string &) const;
};

class BitSampleRate : public Query {
public:
	// Reduces BitRate and SampleRate of a song
	int bits; // 16 by default, new BitRate
	int block; // 1 by default, number of samples that have the same value
	
	BitSampleRate();
	BitSampleRate(const string &, int, int);
	
	int init(bytevector const &, const string &s = "");
	bytevector serialize() const;
	
	void print() const;
	void transform(WAV_File *, const string &) const;
};


/* Raw API */

string sounds_crop(const string &name, const string &new_name, int left, int right);

string sounds_volume(const string &name, const string &new_name, double k, int left, int right, bool smooth);

string sounds_info(const string &name);

