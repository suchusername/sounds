#pragma once
#include "../server.h"
#include "DataSamples.cc"
#include "UniformDataSamples.cc"
#include "AudioFile.cc"
#include "bytevector.cc"
#include "../config.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cerrno>
using namespace std;

static int readStringFromFile(int *data, int pos, int len, string &buf, int fileSize) { // len - length of a string to read
	int blocks = (len + 3) / 4; // == # of blocks to read, 1 block = 4 bytes (int)
	buf = "";
	if (fileSize < pos + blocks - 1) return 1; // file is too short
	int j = 0;
	while(j < blocks) {
		int a = data[pos+j];
		int b = a % (1 << 24); // first byte
		buf += (char) b;
		if ((j == blocks - 1) && (len % 4 == 1)) break; 
		b = (a >> 8) % (1 << 16); // second byte
		buf += (char) b;
		if ((j == blocks - 1) && (len % 4 == 2)) break; 
		b = (a >> 16) % (1 << 8); // third byte
		buf += (char) b;
		if ((j == blocks - 1) && (len % 4 == 3)) break; 
		b = a >> 24; // last byte
		buf += (char) b;
		j++;
	}
	return 0;
}

WAV_File::WAV_File() : AudioFile("", 0, -1, nullptr, 0, 0), Subchunk1Size(0), AudioFormat(0), NumChannels(0), ByteRate(0), BlockAlign(0), BitDepth(0) {}
// constructor for a prototype that is used by object factory
// file description as per http://soundfile.sapp.org/doc/WaveFormat/
	// if PCM then extra parameters do not exists and total data offset from the beginning of the file is 44 bytes (or 11 blocks)
	//The default byte ordering assumed for WAVE data files is little-endian. Files written using the big-endian byte ordering scheme have the identifier RIFX instead of RIFF.
	//The sample data must end on an even byte boundary. Whatever that means.
	//8-bit samples are stored as unsigned bytes, ranging from 0 to 255. 16-bit samples are stored as 2's-complement signed integers, ranging from -32768 to 32767.
	//There may be additional subchunks in a Wave data stream. If so, each will have a char[4] SubChunkID, and unsigned long SubChunkSize, and SubChunkSize amount of data.
	//RIFF stands for Resource Interchange File Format.

WAV_File::~WAV_File() {
	// since the file corresponding to an AudioFile is always opened while the audio is being used, we need to close fd upon destruction and unmap the memory
	// this destructor can also be called through any of the expections
	if (data == MAP_FAILED) { // in this case file was opened but mmap was failed
		close(fd);
	} else if ((data != nullptr) && (fd > 0)) { // in this case file was opened and mmap was successful
		munmap(data, size); // what to do if this fails?
		close(fd);
	} // else data = v.body and fd = -1 (as default)
}

int WAV_File::load(string const &str) {
	/*
	Loads a file that a name 'str' from SoundProject directory
	
	Ex: A.load("Audios/alim.wav") will load a file with path: SoundProject/Audios/alim.wav
	*/
	try {
		bytevector b;
		b.read_from_file(str);
		file_id = str;
		init(b, str);
	} catch (const char *err) {
		throw err;
	}
	
	return 0;
}

int WAV_File::init(bytevector const &v, const string &fcode) {
	/*
	data - container with bytes of WAV file (and maybe code of a file?)
	
	1. Archives a bytevector as a .wav file with a particular (?) name
	2. Initializes all fields of the WAV_File object by parsing bytevector
		- "data" field is initialized as a mmap object (for now)
	
	return value:
		0: successful
		or exception is thrown
	*/
	
	if (fcode.length() == 0) throw "WAV_File::init(): file name cannot be empty.";
	
	//cout << fcode << " " << file_id << endl;
	bool file_exists = (fcode == file_id); // whether needed file already exists (if init() was called from load())
	//php_printf("fcode = %s, file_id = %s, exists = %d<br>", fcode.c_str(), file_id.c_str(), (int) file_exists);
	if (!file_exists) v.write_to_file(fcode);
	
	
	file_id = fcode;
	size = v.size;
	data = (int *) v.body; // temporarily
	
	string fileType, fileFormat;
	int err = readStringFromFile(data, 0, 4, fileType, size);
	if (err) throw "WAV_File::init(): parsing error: unable to read header.";
	
	err = readStringFromFile(data, 2, 4, fileFormat, size);
	if (err) throw "WAV_File::init(): parsing error: unable to read header.";
	
	if ((fileType != "RIFF") && (fileFormat != "WAVE")) {
		throw "WAV_File::init(): parsing error: format is not RIFF WAVE (bytes #0 and #2).";
	}
	
	if (size < 44) {
		throw "WAV_File::init(): parsing error: file is too short.";
	}
	
	if (data[1]+8 != size) {
		throw "WAV_File::init(): parsing error: incorrect file size in WAV file in byte #1.";
	}
	
	string buf, buf_;
	readStringFromFile(data, 3, 4, buf, size); // here size is guaranteed to be correct
	if (buf != "fmt ") throw "WAV_File::init(): parsing error: unable to read header at byte #3.";
	
	this->Subchunk1Size = data[4];
	this->AudioFormat = data[5] % (1 << 16);
	if (AudioFormat != 1) {
		throw "WAV_File::init(): parsing error: audio format is not PCM (Pulse-code modulation) at byte #5.";
	}
	this->NumChannels = data[5] >> 16;
	this->SampleRate = data[6];
	this->ByteRate = data[7];
	this->BlockAlign = data[8] % (1 << 16);
	this->BitDepth = data[8] >> 16;
	if (BitDepth != 16) throw "WAV_File::init(): parsing error: bit depth is not equal to 16 at byte 8.";
	readStringFromFile(data, 9, 4, buf, size);
	
	this->offset = 0;
	if (buf == "LIST") { // space for extra params
		offset = data[10] + 8;
	}
	
	readStringFromFile(data, 9 + (offset/4), 4, buf, size);
	buf = buf.substr(offset % 4, 4 - (offset % 4));
	readStringFromFile(data, 10 + (offset/4), offset % 4, buf_, size);
	buf = buf + buf_;
	//cout << "buf = " << buf << endl;
	if (buf != "data") throw "WAV_File::init(): parsing error: audio format is not PCM (Pulse-code modulation) (at byte #9).";
	
	if (offset % 4 == 0) {
		this->NumSamples = 8 * data[10+offset/4] / NumChannels / BitDepth;
	} else if (offset % 4 == 2) {
		this->NumSamples = (data[10+offset/4] >> 16) + (data[11+offset/4] % (1 << 16) << 16);
		this->NumSamples = 8 * NumSamples / NumChannels / BitDepth;
	}
	
	NumSamples = NumSamples / 2 * 2; // it has to be even because 1 sample = 16 bytes (half of integer)
	
	// all fields, except "fd" and "data" are filled. Now opening a file that was created earlier.
	//cout << fname << endl;
	fd = open(fcode.c_str(), O_RDONLY);
	if (fd < 0) throw "WAV_File::init(): failed to open a file.";
	
	data = (int *)mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
	if (data == MAP_FAILED) {
		close(fd);
		throw "WAV_File::init(): error mmaping the file.";
	}

	return 0;
}

bytevector WAV_File::serialize() const {
	bytevector v;
	string path = FILE_SAVE_DIRECTORY + "/" + file_id + ".wav";
	v.read_from_file(path);
	return v;
}

UniformDataSamples WAV_File::getSamples() const {
	/*
	Returns a container with all data points
	If sound is stereo, then only first channel is considered (TODO: change?)
	*/
	
	UniformDataSamples u(NumSamples);
	u.x_0 = 0;
	u.delta_x = 1 / ((double) SampleRate);
	if (NumChannels == 1) {
		if (offset % 4 == 0) {
			for (int i = 0; i < NumSamples / 2; i++) {
				u[2*i] = (double) (short) (data[11+offset/4+i] % (1 << 16));
				u[2*i+1] = (double) (short) (data[11+offset/4+i] >> 16);
			}
		} else if (offset % 4 == 2) {
			u[0] = (double) (short) (data[11+offset/4] >> 16);
			for (int i = 0; i < NumSamples / 2 - 1; i++) {
				u[2*i+1] = (double) (short) (data[11+offset/4+i] % (1 << 16));
				u[2*i+2] = (double) (short) (data[11+offset/4+i] >> 16);
			}
			u[NumSamples-1] = (double) (short) (data[11 + offset/4 + NumSamples/2 - 1] % (1 << 16));
		} else {
			throw "WAV_File::getSamples(): offset must be even.";
		}
		
	} else if (NumChannels == 2) {
		if (offset % 4 == 0) {
			for (int i = 0; i < NumSamples; i++) {
				u[i] = (double) (short) (data[11+offset/4+i] % (1 << 16));
			}
		}
		if (offset % 4 == 2) {
			for (int i = 0; i < NumSamples; i++) {
				u[i] = (double) (short) (data[11+offset/4+i] >> 16);
				//if (13500 < i && i < 13510) cout << u[i] << endl;
			}			
		} else {
			throw "WAV_File::getSamples(): offset must be even.";
		}
		
	} else {
		throw "WAV_File::getSamples(): Audio can only have 1 or 2 channels.";
	}
	return u;
}


void WAV_File::print() const {
	cout << "--- WAV file information ---" << endl;
	cout << "file_id: " << file_id << endl;
	cout << "size: " << size << " bytes" << endl;
	cout << "Subchunk1Size: " << Subchunk1Size << endl;
	cout << "AudioFormat: " << AudioFormat << endl;
	cout << "NumChannels: " << NumChannels << endl;
	cout << "SampleRate: " << SampleRate << endl;
	cout << "ByteRate: " << ByteRate << endl;
	cout << "BlockAlign: " << BlockAlign << endl;
	cout << "BitDepth: " << BitDepth << endl;
	cout << "offset: " << offset << " (default = 0)" << endl;
	cout << "NumSamples: " << NumSamples << endl;
	cout << "Duration: " << (double) NumSamples / SampleRate << "s" << endl;
}

vector<double> WAV_File::classify() const {
	
	//cout << "file_id: " << file_id << endl;
	pid_t pid = fork();
	if (pid == 0) {
		//string _path = "../" + file_id;
		close(2);
		int fdout = open("logs", O_RDWR | O_CREAT | O_TRUNC, 0666);
		execlp("/Users/apple/miniconda3/bin/python3", "/Users/apple/miniconda3/bin/python3", "../Instrument_classifier/launch.py", file_id.c_str(), NULL);
		perror("execl");
		//php_printf("errno = %d<br>", errno);
		throw "WAV_File::classify(): Error executing python file.";
	} else {
		wait(0);
	}
	
	/*
	instrument_guess:
	0 - accordion
	1 - piano
	2 - violin
	3 - guitar
	4 - noise
	5 - error
	
	guess_prob[0::4]: probabilities of results 0::4 respectively
	*/
	
	int instrument_guess;
	vector<double> guess_prob(5);
	
	
	
	ifstream in_file(ANSWER_FILE_NAME.c_str(), ios::in);
	if (!in_file.is_open()) throw "WAV_File:: unable to open the file with answers.";
	in_file >> instrument_guess;
	for (int i = 0; i < 4; i++) in_file >> guess_prob[i];
	in_file.close();
	
	return guess_prob;
}



