	#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Admin.h"

using namespace std;

void printHelp();

int main(int argc, const char * argv[]) {
	int processTo = 0;
	bool verbose = false, verboseChanged = false, fileOutput = false,
			errFileOutput = false, astOutput = false, unattended = false;
	string cmd, outputFile = "", errOutputFile = "";
	vector<string> filenames;

	if(argc < 2) {
		cout << "No input specified" << endl;
		return 1;
	}

	// Start looking for command line arguments, exclude last index at least
	for(int i = 1; i < argc; i++) {
		cmd = argv[i];

		if(cmd == "-h" || cmd == "-help") {
			printHelp();
		}
		
		// Phase selection
		else if(cmd == "-l" || cmd == "-lex") {
			if(processTo != 0) {
				processTo = 1;
			}
			else {
				cout << "Cannot specify multiple phases to process up to" << endl;
				return 7;
			}
		}
		else if(cmd == "-p" || cmd == "-parse") {
			if(processTo != 0) {
				processTo = 2;
			}
			else {
				cout << "Cannot specify multiple phases to process up to" << endl;
				return 7;
			}
		}
		// Disabled until they are implemented
		else if(cmd == "-s" || cmd == "-sem" || cmd == "-t" || cmd == "-tup" ||
				cmd == "-c" || cmd == "-compile") {
			cout << "Selected phases not yet implemented" << endl;
			return 8;
		}
		/*else if(cmd == "-s" || cmd == "-sem") {
			if(processTo != 0) {
				processTo = 3;
			}
			else {
				cout << "Cannot specify multiple phases to process up to" << endl;
				return 7;
			}
		}
		else if(cmd == "-t" || cmd == "-tup") {
			if(processTo != 0) {
				processTo = 4;
			}
			else {
				cout << "Cannot specify multiple phases to process up to" << endl;
				return 7;
			}
		}
		else if(cmd == "-c" || cmd == "-compile") {
			if(processTo != 0) {
				processTo = 5;
			}
			else {
				cout << "Cannot specify multiple phases to process up to" << endl;
				return 7;
			}
		}*/
		
		else if(cmd == "-ast") {
			astOutput = true;
		}
		
		// Quiet mode
		else if(cmd == "-q" || cmd == "-quiet") {
			if(verboseChanged) {
				cout << "Cannot specify both quiet and verbose mode" << endl;
				return 9;
			}
			verbose = false;
			verboseChanged = true;
		}
		// Verbose mode
		else if(cmd == "-v" || cmd == "-verbose") {
			if(verboseChanged) {
				cout << "Cannot specify both quiet and verbose mode" << endl;
				return 9;
			}
			verbose = true;
			verboseChanged = true;
		}
		
		// No user-involvement required
		// Parameter hidden from help screen, only used for testing
		else if(cmd == "-u") {
			unattended = true;
		}
		
		// Output redirect
		else if(cmd == "-o" || cmd == "-out") {
			fileOutput = true;
			if(argc < i+2) {
				cout << "Missing parameter: output stream" << endl;
				return 5;
			}
			outputFile = argv[i+1];
			i++;
		}
		// Error redirect
		else if(cmd == "-e" || cmd == "-err") {
			errFileOutput = true;
			if(argc < i+2) {
				cout << "Missing parameter: error output stream" << endl;
				return 5;
			}
			errOutputFile = argv[i+1];
			i++;
		}
		
		// Otherwise it is a source file name
		else {
			filenames.push_back(cmd);
		}
	}
	
	if(filenames.size() == 0) {
		cout << "No source files specified" << endl;
		return 10;
	}

	if(!unattended && filenames.size() > 0) {
		cout << "CAD Compiler v1.2" << endl;
		cout << "Source files: " << endl;
		for(int i = 0; i < filenames.size(); i++) {
			cout << "  " << filenames[i] << endl;
		}
		if(verbose) {
			cout << "Verbose mode enabled (trace enabled)" << endl;
		}
		if(fileOutput) {
			cout << "Output redirected to file: " << outputFile << endl;
		}
		if(astOutput) {
			cout << "  AST will be included after output" << endl;
		}
		if(errFileOutput) {
			cout << "Error output redirected to file: " << errOutputFile << endl;
		}
		
		// Only processing certain phases
		if(processTo != 0 && processTo != 5) {
			cout << "Processing up to ";
		}
		switch(processTo) {
			case 1:
				cout << "scanner";
				break;
			case 2:
				cout << "parser";
				break;
			case 3:
				cout << "semantic analysis";
				break;
			case 4:
				cout << "tuple";
				break;
		}
		if(processTo != 0 && processTo != 5) {
			cout << " phase" << endl;
		}
	}
	
	// Make sure these get appended for multiple files
    ofstream output(outputFile);
	ofstream errOutput(errOutputFile);

	// Start up the compiler
	for(int i = 0; i < filenames.size(); i++) {
		ifstream file(filenames[i]);
		
		if(!file.is_open()) {
			cout << "Could not find source file: " << filenames[i] << endl;
			continue;
		}
		
		if(!unattended) {
			output << "-- Starting " << filenames[i] << endl << endl;
		}
		
		Admin admin(file, filenames[i], cout, verbose);
		admin.setOutputAST(astOutput);

		if(fileOutput) {
			admin.setOutputStream(output);
			if(!errFileOutput) {
				admin.setErrOutputStream(output);
			}
		}
		if(errFileOutput) {
			admin.setErrOutputStream(errOutput);
		}
	
		admin.compile(processTo);
		
		if(!unattended) {
			output << endl << endl << "-- Ending " << filenames[i] << endl << endl;
		}
	}
	
    output.close();
	errOutput.close();

	return 0;
}

/* Print help information for the user
 */
void printHelp() {
	cout << "Usage:" << endl;
	cout << "[bin]/cs13 [options] file1 file2 ..." << endl;
	cout << endl;
	cout << "Option Listing:" << endl;
	cout << "-h | -help     -- Displays this help menu" << endl;
	cout << "-l | -lex      -- Process up to the Lexer phase" << endl;
	cout << "-p | -parse    -- Process up to the Parser phase" << endl;
	cout << "-s | -sem      -- Process up to the Semantic Analysis phase" << endl;
	cout << "-t | -tup      -- Process up to the Tuple phase" << endl;
	cout << "-c | -compile  -- Process all phases and compile (default behavior)" << endl;
	cout << "-ast			-- Output the AST (directed to 'out' option)" << endl;
	cout << "-q | -quiet    -- Only display error messages (default behavior)" << endl;
	cout << "-v | -verbose  -- Display all trace messages" << endl;
	cout << "-o | -out      -- Output file (defaults to stdout)" << endl;
	cout << "-e | -err      -- Error file (defaults to stdout)" << endl;
	cout << endl;
	cout << "Examples:" << endl;
	cout << "cs13 -l -v test1.txt test.txt  -- Scan test1.txt and test2.txt verbosely" << endl;
	cout << "cs13 -o myapp myapp.txt        -- Compile myapp.txt to executable myapp" << endl;
}
