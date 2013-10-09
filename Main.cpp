#include <fstream>
#include <iostream>
#include <string>
#include "Admin.h"

using namespace std;

// !!!!!!!!!!!!!!!!!!!!!!!!!!
// Made change to scanner get token!
// Also fixed the ENDFILE issue!

int main(int argc, const char * argv[]) {
	bool verbose = false, fileOutput = false, unattended = false;
	string cmd, filename = "", outputFile = "";

	if(argc < 2) {
		cout << "No input specified";
		return 1;
	}

	// Start looking for command line arguments
	for(int i = 1; i < argc; i++) {
		cmd = argv[i];

		// Source file
		if(cmd == "-s") {
			if(argc < i+2) {
				cout << "Missing parameter: filename";
				return 2;
			}
			filename = argv[i+1];
			i++;
		}
		// Verbose mode
		else if(cmd == "-v") {
			verbose = true;
		}
		// No user-involvement required
		else if(cmd == "-u") {
			unattended = true;
		}
		// Output redirect
		else if(cmd == "-o") {
			fileOutput = true;
			if(argc < i+2) {
				cout << "Missing parameter: output";
				return 5;
			}
			outputFile = argv[i+1];
			i++;
		}
		else {
			cout << "Invalid parameter: " << cmd;
			return 3;
		}
	}

	if(!unattended) {
		cout << "CAD Scanner v1.1 Loaded" << endl;
		if(filename != "") {
			cout << "Source file: " << filename << endl;
		}
		if(verbose) {
			cout << "Verbose mode enabled (trace enabled)" << endl;
		}
		if(fileOutput) {
			cout << "Output redirected to file: " << outputFile << endl;
		}
	}

	// Filename is required to use compiler
	if(filename == "") {
		cout << "No filename was specified. Use the \"-s\" parameter followed by a filename.";
		return 4;
	}

	// Start up the compiler
    ifstream file(filename);
	if(!file.is_open()) {
		cout << "Could not find source file: " << filename << endl;
		return 6;
	}
    ofstream output(outputFile);
	Admin admin(file, cout, verbose);
    if(fileOutput) {
        admin.setOutputStream(output);
	}
	admin.compile();
    output.close();

	return 0;
}
