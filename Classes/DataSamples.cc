#pragma once
#include "../server.h"

#include "bytevector.cc"
#include <vector>

using namespace std;

DataSamples::DataSamples(int n) : N(n) {
	if (n == 0) {
		y = nullptr;
	} else {
		y = new double[n]();
	}
}

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
