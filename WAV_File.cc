#pragma once
#include <config.h>
#include "bytevector.cc"
using namespace std;

AudioFile::AudioFile(string file_id, int size, int fd, int *data, int NumSamples, int SampleRate) : 
file_id(file_id), size(size), fd(fd), data(data), NumSamples(NumSamples), SampleRate(SampleRate) {}

WAV_File::WAV_File() : AudioFile("", 0, -1, nullptr, 0, 0), Subchunk1Size(0), AudioFormat(0), NumChannels(0), ByteRate(0), BlockAlign(0), BitDepth(0) {}

WAV_File::~WAV_File() {
	
}

int WAV_File::getObjId() {
	return 0;
}

int WAV_File::init(bytevector v) {
	return 0;
}

bytevector WAV_File::serialize() const {
	bytevector v;
	return v;
}

WAV_File * WAV_File::clone() const {
	return this;
}
