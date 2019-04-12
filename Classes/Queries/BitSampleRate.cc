#pragma once
#include "../../server.h"
#include "../bytevector.cc"
#include "../Query.cc"
#include "Volume.cc"

#include <string>
#include <iostream>
using namespace std;

BitSampleRate::BitSampleRate() : Query(), bits(16), block(1) {}

BitSampleRate::BitSampleRate(const string &s, int bits, int block) : Query(s), bits(bits), block(block) {}

int BitSampleRate::init(bytevector const &v, const string &s) {
	throw "BitSampleRate::init(): not implemented.";
	return 0;
}

bytevector BitSampleRate::serialize() const {
	/*
	8 bytes: file_id (unsigned long long)
	4 bytes: bits (int)
	4 bytes: block (int)
	*/
	
	throw "BitSampleRate::serialize(): not implemented.";
	
	bytevector v;
	return v;	
}

void BitSampleRate::print() const {
	cout << "file_id = " << file_id << endl;
	cout << "bits = " << bits << ", block = " << block << endl;
}

void BitSampleRate::transform(WAV_File *file, const string &new_id) const {
	/*
	Performs transformation of a file.
	Saves it under name new_id.
	
	Multiplies the signal by such an amount that there are no overflows and reduces bit rate and sample rate.
	*/
	
	if ((bits < 1) || (bits > 16)) throw "BitSampleRate::transform(): invalid bit rate.";
	if (block < 1) throw "BitSampleRate::transform(): invalid block size.";
	
	UniformDataSamples arr = file->getSamples();
	
	int max = 0; // max absolute value
	for (int i = 0; i < arr.N; i++) {
		if (arr[i] > max) max = arr[i];
		if (arr[i] < -max) max= -arr[i];
	}
	double k = (double) 28000 / (double) max;
	Volume V("", k);
	V.transform(file, "1");
	
	for (int i = 0; i < arr.N; i++) {
		arr[i] = ((int) arr[i] >> (16-bits)) << (16-bits);
		arr[i] = arr[i - (i % block)];
	}
		
	bytevector b(44 + 2*arr.N);
	b.writeString("RIFF", 0);
	b.writeInt(36+2*arr.N, 4); // new size
	b.writeString("WAVE", 8);
	b.writeString("fmt ", 12);
	b.writeInt(file->Subchunk1Size, 16);
	writeShort(file->AudioFormat, b, 20);
	writeShort(1, b, 22); // NumChannels
	b.writeInt(file->SampleRate, 24);
	b.writeInt(file->ByteRate, 28);
	writeShort(file->BlockAlign, b, 32);
	writeShort(file->BitDepth, b, 34);
	b.writeString("data", 36);
	b.writeInt(2*arr.N, 40);
	
	for (int i = 0; i < arr.N; i++) writeShort((int) arr[i], b, 44+2*i);
	
	file->init(b, new_id);	
}




