#pragma once
#include "../Classes/server.h"

#include "../Classes/bytevector.cc"
#include <vector>
#include <iostream>

using namespace std;

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
	return 0;
}

bytevector UniformDataSamples::serialize() const {
	bytevector v;
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

