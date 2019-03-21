#include <stdio.h>
#include <string>
#include "bytevector.cc"
#include "WAV_File.cc"

int main() {
	
	try {
		string fname = "sound.wav";
		bytevector V;
		V.read_from_file(fname);
		//V.print();
		WAV_File file;
		
		
		file.init(V);
		file.print();
		
	} catch(const char *err) {
		printf("%s\n", err);
	}
	
}