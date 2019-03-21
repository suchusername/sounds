#include <stdio.h>
#include <string>
#include "bytevector.cc"

int main() {
	string y = "abc";
	bytevector x(y.c_str(), 3);
	bytevector xx;
 	x.print();
	
	string name = "test.txt";
	try {
		x.write_to_file(name);
		xx.print();
		xx.read_from_file(name);
	} catch(const char *err) {
		printf("%s\n", err);
	}
	xx.print();
	
	
}