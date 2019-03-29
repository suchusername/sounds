#pragma once
#include "../server.h"
#include "bytevector.cc"
#include "DataSamples.cc"

#include <math.h>
#include <vector>
#include <iostream>

using namespace std;

static void writeInt(int x, bytevector &b, int pos) {
	int x_ = x + (1 << 30);
	for (int i = 0; i < 4; i++) {
		b[pos+i] = (char) (x_ % 256);
		x_ >>= 8;
	}
}
static int readInt(bytevector const &b, int pos) {
	int N_ = 0;
	for (int i = 0; i < 4; i++) {
		N_ += ((256 + (int) b[pos+i]) % 256) << (8*i);
	}
	return N_ - (1 << 30);
}

static void writeDouble(double x, bytevector &b, int pos) {
	if (pos+7 >= b.size) throw "UniformDataSamples::serialize(): error writing double to bytevector";
	if ((x >= 1e9) || (x <= 1-1e9)) throw "UniformDataSamples::serialize(): double out of range";
	int X = floor(x);
	writeInt(X, b, pos);
	int Y = floor((x - X) * 1e9);
	writeInt(Y, b, pos+4);
}

static double readDouble(bytevector const &b, int pos) {
	double a1 = (double) readInt(b, pos);
	double a2 = (double) readInt(b, pos+4);
	//cout << readInt(b, pos) << " " << readInt(b, pos+4) << endl;
	return a1 + a2 * 1e-9;
}

UniformDataSamples::UniformDataSamples(int n) : DataSamples(n), x_0(0), delta_x(0) {}

UniformDataSamples::UniformDataSamples(const double *y, int n, double x_0, double d) : DataSamples(y, n), x_0(x_0), delta_x(d) {}

UniformDataSamples::UniformDataSamples(vector<double> const &y, double x_0, double d) : DataSamples(y), x_0(x_0), delta_x(d) {}

UniformDataSamples::UniformDataSamples(UniformDataSamples const &vec) {
	N = vec.N;
	x_0 = vec.x_0;
	delta_x = vec.delta_x;
	y = new double[N];
	for (int i = 0; i < N; i++) y[i] = vec.y[i];	
}

UniformDataSamples & UniformDataSamples::operator=(UniformDataSamples const &vec) {
	N = vec.N;
	x_0 = vec.x_0;
	delta_x = vec.delta_x;
	if (y != nullptr) delete [] y;
	y = new double[N];
	for (int i = 0; i < N; i++) y[i] = vec.y[i];
	return *this;
}

double & UniformDataSamples::operator[](int k) {
	if ((k < 0) || (k >= N)) throw "UniformDataSamples::operator[](): index out of range";
	return y[k];
}

double UniformDataSamples::operator[](int k) const {
	if ((k < 0) || (k >= N)) throw "UniformDataSamples::operator[](): index out of range";
	return y[k];
}

UniformDataSamples::~UniformDataSamples() {
	if (y != nullptr) delete [] y;
}

int UniformDataSamples::getObjId() const {
	return 0;
}

int UniformDataSamples::init(bytevector const &v) {
	/*
	Initializes an object UniformDataSamples from its bytevector. Format is defined below.
	
	Return value:
		0: success
		or the exception is thrown
	*/
	
	if (v.size < 4) throw "UniformDataSamples::init(): bytevector is too short (cannot read N)";
	int N_ = readInt(v, 0);
	if (v.size < 20+8*N_) throw "UniformDataSamples::init(): bytevector is too short";
	N = N_;
	x_0 = readDouble(v, 4);
	delta_x = readDouble(v, 12);
	
	if (y != nullptr) delete [] y;
	y = new double[N];
	for (int i = 0; i < N; i++) y[i] = readDouble(v, 20+8*i);
	
	return 0;
}

bytevector UniformDataSamples::serialize() const {
	/*
	Creates a bytevector - compressed version of UniformDataSamples object. 
	TODO: change format: different architectures have different double formats (possible send as a string?)
	
	Only works for doubles with absolute value < 10^9!
	
	Format:
	4 bytes: N (int)
	8 bytes: x_0 (4+4 bytes)
	8 bytes: delta_x (4+4 bytes)
	(4+4)*N bytes:
		4 bytes: integer part of a double
		4 bytes: decimal part of a double
	
	Return value:
	bytevector object
	*/
	
	bytevector v(20+8*N);
	
	writeInt(N, v, 0);
	writeDouble(x_0, v, 4);
	writeDouble(delta_x, v, 12);
	for (int i = 0; i < N; i++) {
		writeDouble(y[i], v, 20+8*i);
	}
		
	return v;
}

UniformDataSamples * UniformDataSamples::clone() const {
	UniformDataSamples *u;
	return u;
}

void UniformDataSamples::print() const {
	cout << "N = " << N << ", x_0 = " << x_0 << ", delta_x = " << delta_x << endl;
	cout << "y:";
	for (int i = 0; i < N; i++) cout << " " << y[i];
	cout << endl;
}
