#include <stdio.h>
#include <string>
#include <vector>
#include "Classes/bytevector.cc"
#include "Classes/WAV_File.cc"
#include "Algorithms/DataSamples.cc"

int main() {
	double *b = new double[3];
	b[0] = 0.1;
	b[1] = 0.2;
	b[2] = 0.4;
	
	vector<double> a(3);
	a[0] = 1.32434345934;
	a[1] = 0.432432232;
	a[2] = 3.1416;
	
	
	UniformDataSamples A(a, 0, 1);
	UniformDataSamples B(b, 3, 0, 1);
	UniformDataSamples C(A);
	A.print();
	B.print();
	C.print();
	
	C = B;
	C.print();
	
	delete [] b;
}