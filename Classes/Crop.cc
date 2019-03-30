#pragma once
#include "../server.h"
#include "bytevector.cc"
#include "Query.cc"

#include <string>
#include <iostream>
using namespace std;

Crop::Crop() : Query(), left(0), right(0) {}

Crop::Crop(string s, int l, int r) : Query(s), left(l), right(r) {}

int Crop::getObjId() const {
	//TODO
	return 0;
}

int Crop::init(bytevector const &v, const string &s) {
	if (v.len() != 16) throw "Crop::init(): bytevector size must be 16 bytes";
	file_id = to_string(readUll(v, 0));
	left = readInt(v, 8);
	right = readInt(v, 12);	
	return 0;
}

bytevector Crop::serialize() const {
	/*
	8 bytes: file_id (unsigned long long)
	4 bytes: left (int)
	4 bytes: right (int)
	*/
	
	if (!is_number(file_id)) throw "Crop::serialize(): file_id must be an integer";
	
	bytevector v(16);
	writeUll(stoull(file_id), v, 0);
	writeInt(left, v, 8);
	writeInt(right, v, 12);
	return v;	
}

Crop * Crop::clone() const {
	Crop *c;
	return c;
}

void Crop::print() const {
	cout << "file_id = " << file_id << endl;
	cout << "left = " << left << ", right = " << right << endl;
}

void Crop::transform(WAV_File *file, const string &new_id) const {
	/*
	Performs transformation of a file.
	Saves it under name new_id.
	Only works for mono files.
	*/
	if (!is_number(new_id)) throw "Crop::transform(): new file id must be an integer";
	
	UniformDataSamples arr = file->getSamples();
	//cout << arr[10022] << endl;
	arr.crop(left, right, 0);
	
	//arr.print();
		
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




