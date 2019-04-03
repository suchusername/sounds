#pragma once
#include "../../server.h"
#include "../bytevector.cc"
#include "../Query.cc"

#include <math.h>
#include <string>
#include <iostream>
using namespace std;

void Volume::writeInt(int x, bytevector &b, int pos) const {
	int x_ = x + (1 << 30);
	for (int i = 0; i < 4; i++) {
		b[pos+i] = (char) (x_ % 256);
		x_ >>= 8;
	}
}
int Volume::readInt(bytevector const &b, int pos) const {
	int N_ = 0;
	for (int i = 0; i < 4; i++) {
		N_ += ((256 + (int) b[pos+i]) % 256) << (8*i);
	}
	return N_ - (1 << 30);
}

void Volume::writeDouble(double x, bytevector &b, int pos) const {
	if (pos+7 >= b.size) throw "UniformDataSamples::serialize(): error writing double to bytevector";
	if ((x >= 1e9) || (x <= 1-1e9)) throw "UniformDataSamples::serialize(): double out of range";
	int X = floor(x);
	writeInt(X, b, pos);
	int Y = floor((x - X) * 1e9);
	writeInt(Y, b, pos+4);
}

double Volume::readDouble(bytevector const &b, int pos) const {
	double a1 = (double) readInt(b, pos);
	double a2 = (double) readInt(b, pos+4);
	//cout << readInt(b, pos) << " " << readInt(b, pos+4) << endl;
	return a1 + a2 * 1e-9;
}


Volume::Volume() : Query(), k(1) {}

Volume::Volume(const string &s, double k) : Query(s), k(k) {}

int Volume::getObjId() const {
	//TODO
	return 0;
}

int Volume::init(bytevector const &v, const string &s) {
	if (v.len() != 16) throw "Volume::init(): bytevector size must be 16 bytes";
	file_id = to_string(readUll(v, 0));
	k = readDouble(v, 8);
	return 0;
}

bytevector Volume::serialize() const {
	/*
	8 bytes: file_id (unsigned long long)
	8 bytes: k (4+4 bytes, integer part of double and decimal part)
	*/
	
	if (!is_number(file_id)) throw "Volume::serialize(): file_id must be an integer";
	
	bytevector v(16);
	writeUll(stoull(file_id), v, 0);
	writeDouble(k, v, 8);
	return v;	
}

Volume * Volume::clone() const {
	Volume *c;
	return c;
}

void Volume::print() const {
	cout << "file_id = " << file_id << endl;
	cout << "k = " << k << endl;
}

void Volume::transform(WAV_File *file, const string &new_id) const {
	/*
	Performs transformation of a file.
	Saves it under name new_id.
	*/
	if (!is_number(new_id)) throw "Crop::transform(): new file id must be an integer";
	
	UniformDataSamples arr = file->getSamples();
	
	for (int i = 0; i < file->NumSamples; i++) {
		arr[i] = (int) (arr[i] * k);
		if (arr[i] > 32767) arr[i] = 32767;
		if (arr[i] < -32767) arr[i] = 32767;
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




