#pragma once
#include "../server.h"

#include "bytevector.cc"
#include <vector>
#include <iostream>

using namespace std;

static void writeDouble(double x, bytevector &b, int pos) {
	if (pos+7 >= b.size) throw "DataSamples::serialize(): error writing double to bytevector";
	unsigned long long X = (unsigned long long) x;
	for (int i = 0; i < 8; i++) {
		b[pos+i] = (char) (X % 256);
		X >>= 8;
	}
}

static double readDouble(bytevector const &b, int pos) {
	if (pos+7 >= b.size) throw "DataSamples::init(): error reading double from bytevector";
	unsigned long long X = 0;
	for (int i = 0; i < 8; i++) {
		X += (unsigned long long) b[pos+i] << (8*i);
	}
	return (double) X;
}

DataSamples::DataSamples() : N(0), y(nullptr) {}

DataSamples::DataSamples(const double *vals, int n) : N(n) { // we are guaranteed that n is size of const double*
	if (n == 0) {
		y = nullptr;
	} else {
		y = new double[n];
		for (int i = 0; i < n; i++) y[i] = vals[i];	
	}
}

DataSamples::DataSamples(vector<double> const &vals) : N(vals.size()) {
	if (vals.size() == 0) {
		y = nullptr;
	} else {
		y = new double[N];
		for (int i = 0; i < N; i++) y[i] = vals[i];	
	}
}

UniformDataSamples::UniformDataSamples() : DataSamples(), x_0(0), delta_x(0) {}

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
	int N_ = 0;
	for (int i = 0; i < 4; i++) {
		N_ += ((int) v[i]) << (8*i);
	}
	cout << N_ << endl;
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
	
	Format:
	4 bytes: N (int)
	8 bytes: x_0 (double)
	8 bytes: delta_x (double)
	8*N bytes: values of y (double)
	
	Return value:
	bytevector object
	*/
	
	bytevector v(20+8*N);
	int N_ = N;
	for (int i = 0; i < 4; i++) {
		v[i] = (char) (N_ % 256);
		N_ >>= 8;
	}
	writeDouble(x_0, v, 4);
	writeDouble(delta_x, v, 12);
	for (int i = 0; i < N; i++) writeDouble(y[i], v, 20+8*i);
		
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

