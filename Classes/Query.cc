#pragma once
#include "../server.h"
#include "bytevector.cc"

#include <iostream>
#include <string>
using namespace std;

void Query::writeUll(unsigned long long x, bytevector &b, int pos) const {
	unsigned long long x_ = x;
	for (int i = 0; i < 8; i++) {
		b[pos+i] = (char) (x_ % 256);
		x_ >>= 8;
	}
}
unsigned long long Query::readUll(bytevector const &b, int pos) const {
	unsigned long long N_ = 0;
	for (int i = 0; i < 8; i++) {
		N_ += ((256 + (unsigned long long) b[pos+i]) % 256) << (8*i);
	}
	return N_;
}
void Query::writeInt(int x, bytevector &b, int pos) const { // only for personal use
	int x_ = x + (1 << 30);
	for (int i = 0; i < 4; i++) {
		b[pos+i] = (char) (x_ % 256);
		x_ >>= 8;
	}
}
int Query::readInt(bytevector const &b, int pos) const { // only for personal use
	int N_ = 0;
	for (int i = 0; i < 4; i++) {
		N_ += ((256 + (int) b[pos+i]) % 256) << (8*i);
	}
	return N_ - (1 << 30);
}

void Query::writeShort(int x, bytevector &b, int pos) const {
	//cout << x << " ";
	if ((x < -(1 << 15)) || (x >= (1 << 15))) throw "Query::writeShort(): input is not of type 'short'";
	int x_ = x;
	for (int i = 0; i < 2; i++) {
		//cout << (256 + (x_ % 256)) % 256 << " ";
		b[pos+i] = (char) ((256 + (x_ % 256)) % 256);
		x_ >>= 8;
	}
	//cout << endl;
}

bool Query::is_number(const string & s) const {
    return !s.empty() && find_if(s.begin(), s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

Query::Query() {}

Query::Query(string s) : file_id(s) {}
