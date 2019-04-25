#pragma once
#include "../../server.h"
#include "../bytevector.cc"
#include "../Query.cc"

#include <string>
#include <iostream>
using namespace std;

Speed::Speed() : Query(), mult(1) {}

Speed::Speed(const string &s, double m) : Query(s), mult(m) {}

int Speed::init(bytevector const &v, const string &s) {
	throw "Speed::init(): not implemented.";
	return 0;
}

bytevector Speed::serialize() const {
	throw "Speed::serialize(): not implemented.";
	bytevector b;
	return b;
}

void Speed::print() const {
	cout << "file_id = " << file_id << endl;
	cout << "mult = " << mult << endl;
}

void Speed::transform(WAV_File *file, const string &new_id) const {
	/*
	Performs transformation of a file.
	Saves it under name new_id.
	*/
	
	// Получет массив из точек аудиозаписи
	UniformDataSamples arr = file->getSamples();
	// Само преобразование массив arr
	
	
	// Запись нового файла
	// Начать катать
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
	// Закончить катать
}




