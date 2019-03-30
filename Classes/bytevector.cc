#pragma once
#include "../server.h"

#include <string>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <iostream>
using namespace std;

long bytevector::FdGetFileSize(int fd) {
    struct stat stat_buf;
    int rc = fstat(fd, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

bytevector::bytevector(int n) : size(n) {
	if (n == 0) {
		body = nullptr;
	} else {
		body = new char[n]();
	}
}

bytevector::bytevector(const char *A, int n) {
	size = n;
	body = new char[n];
	for (int i = 0; i < n; i++) body[i] = A[i];
}

bytevector::bytevector(string const &str) {
	size = str.length();
	body = new char[size];
	for (int i = 0; i < size; i++) body[i] = str[i];
}

bytevector::~bytevector() {
	if (size > 0) {
		delete [] body;
	}
}

char bytevector::operator[](int k) const {
	if (k >= size) throw "bytevector::operator[]() const: index out of range";
	return body[k];
}

char & bytevector::operator[](int k) {
	if (k >= size) throw "bytevector::operator[](): index out of range";
	return body[k];
}

int bytevector::write_to_file(string const &name) const {
	/* 
	name - name of a file created
	
	creates a file called "name" in FILE_SAVE_DIRECTORY
	
	return value:
		1: file not created (size = 0)
		0: file created successfully
		or exception is thrown
	*/
	
	//printf("bytevector::write_to_disc(%s)\n", name.c_str());
	if (size == 0) return 1;
	
	//string path = FILE_SAVE_DIRECTORY + "/" + name;
	string path = name;
	//cout << path << endl;
	int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd < 0) throw "bytevector::write_to_file(): could not create a file.";
	
	int err = write(fd, body, size);
	if (err < 0) {
		close(fd);
		throw "bytevector::write_to_file(): error writing to file.";
	}
	
	close(fd);
	return 0;
}

int bytevector::len() const {
	return size;
}

int bytevector::read_from_file(string const &name) {
	/*
	name - name of a file that is opened
	
	writes contents of a file "name" from FILE_SAVE_DIRECTORY in body of bytevector
	
	return value:
		0: file read successfully
		or exception is thrown
	*/
	//string path = FILE_SAVE_DIRECTORY + "/" + name;
	string path = name;
	
	int fd = open(path.c_str(), O_RDONLY, 0666);
	if (fd < 0) throw "bytevector::read_from_file(): could not open a file.";
	
	int fsize = FdGetFileSize(fd);
	if (fsize < 0) {
		close(fd);
		throw "bytevector::read_from_file(): could not get file size.";
	}
	
	if (fsize == 0) {
		if (body != nullptr) {
			delete [] body;
			body = nullptr;
		}
		size = 0;
		close(fd);
		return 0;
	}
	
	size = fsize;
	if (body != nullptr) delete [] body;
	body = new char[size];
	
	int err = read(fd, body, size);
	if (err < 0) {
		close(fd);
		throw "bytevector::read_from_file(): error reading from a file.";
	}
	
	close(fd);
	return 0;
}

void bytevector::writeString(const string &str, int pos) {
	if (pos + str.length() > size) throw "bytevector::writeString(): a string is too long.";
	for (int i = 0; i < str.length(); i++) body[pos+i] = str[i];
}

void bytevector::writeInt(int x, int pos) { // writes in little-endian
	if (pos + 4 > size) throw "bytevector::writeInt(): integer does not fit.";
	int x_ = x;
	for (int i = 0; i < 4; i++) {
		body[pos+i] = (char) (x_ % 256);
		x_ >>= 8;
	}
}

void bytevector::print() const {
	printf("size = %d, body = ", size);
	for (int i = 0; i < size; i++) printf("%c", body[i]);
	printf("\n");
}




