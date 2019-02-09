#pragma once

#ifndef CONFIG_H
#include "config.h"
#endif

#include <string>
#include <map>
#include <iostream>
using namespace std;

/*
A class to store variable in a map.
Useful when parameters come through shell's command line in format:
>>> ./prog path/to/file.wav param1=val1 param2=val2
*/

class ArgMap {
private:
	map<string, string> args;
	
public:
	ArgMap() : args() {}
	
	void fill(int argc, char **argv) {
		args["file"] = DEFAULT_FILE_NAME;
		args["samples_left"] = SAMPLES_TO_SHOW_DFL_LEFT;
		args["samples_right"] = SAMPLES_TO_SHOW_DFL_RIGHT;
		args["freq_left"] = LEFT_FREQ_BORDER_DFL;
		args["freq_right"] = RIGHT_FREQ_BORDER_DFL;
		
		
		bool audioPathSet = false;
		for (int i = 1; i < argc; i++) {
			string s(argv[i]);
			int pos = s.find('=');
			if ((pos == string::npos) && (!audioPathSet)) {
				audioPathSet = true;
				args["file"] = s;
			} else if (pos == string::npos) {
				throw "ERROR: params.cc: incorrect input format, audio path file already set more than once.\n";
			} else {
				string ss = s.substr(0, pos);
				if (ss == "file") {
					throw "ERROR: params.cc: incorrect input format.\n";
				}
				s = s.erase(0, pos+1);
				args[ss] = s;
			}
		}
		return;
	}
	
	string get(char *s) {
		string str(s);
		return args[str];
	}
};