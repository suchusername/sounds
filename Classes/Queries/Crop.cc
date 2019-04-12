#pragma once
#include "../../server.h"
#include "../bytevector.cc"
#include "../Query.cc"

#include <string>
#include <iostream>
using namespace std;

Crop::Crop() : Query(), left(0), right(0) {}

Crop::Crop(const string &s, int l, int r) : Query(s), left(l), right(r) {}

int Crop::init(bytevector const &v, const string &s) {
	throw "Crop::init(): not implemented.";
	return 0;
}

bytevector Crop::serialize() const {
	throw "Crop::serialize(): not implemented.";
	bytevector b;
	return b;
}

void Crop::print() const {
	cout << "file_id = " << file_id << endl;
	cout << "left = " << left << ", right = " << right << endl;
}

void Crop::transform(WAV_File *file, const string &new_id) const {
	/*
	Performs transformation of a file.
	Saves it under name new_id.
	*/
	
	UniformDataSamples arr = file->getSamples();
	arr.crop(left, right, 0);
		
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




