#ifndef Admin_H
#define Admin_H

#include <fstream>
#include <ostream>
#include <vector>
class Scanner;
class Parser;
class Token;

#define PARSER_ENTER 0
#define PARSER_EXIT 1

using namespace std;

/* The Admin class is responsible for reading source files and writing to output streams.
 * It also performs white space and invisible character filtering on the input stream.
 * It is generally an interface between files and the compiler.
 */
class Admin
{
private:
	int linePos, lineCount;
	bool traceScanner, traceParser, almostDone;
	string line;
	ifstream * source;
	ostream * output;
	Scanner* sc;
	Parser* ps;
	
	bool isWhiteSpace(char c);
	bool isInvisibleCharacter(char c);
public:
	vector<Token> vec;

	// Constructor/deconstructors/related
	Admin(void);
	Admin(ifstream & file, ostream & out);
	Admin(ifstream & file, ostream & out, bool traceEnabled);
	Admin(const Admin &other);
	Admin& operator= (const Admin& rhs);
	~Admin(void);

	// Getters/setters
	void setOutputStream(ostream & out);
	
	// Other functions
	void compile();
	char getCh(bool skipWs);
	void endLine();
	void scannerLog();
	void scannerLogEnd();
        void parserLog(string functionName, int mode);
	void unget();
        void syntaxError(int expected);
};

#endif
