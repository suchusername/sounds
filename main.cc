#include <stdio.h>
#include <string>
#include <vector>
#include "Classes/bytevector.cc"
#include "Classes/WAV_File.cc"
#include "Classes/UniformDataSamples.cc"

int main() {	
	try {
		vector<double> a(3);
		a[0] = 2.62434345934;
		a[1] = 0.432432232;
		a[2] = 3.1416;
		
		UniformDataSamples A(a, 0, 1);
		A.print();
	
		bytevector v = A.serialize();
		UniformDataSamples B;
		B.print();
		B.init(v);
		B.print();
	} catch(const char *err) {
		printf("%s\n", err);
	}
}