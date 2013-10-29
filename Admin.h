#ifndef Admin_H
#define Admin_H

#include <fstream>
#include <ostream>
#include <vector>
#include "ASTNode.h"
#include "SemanticAnalyzer.h"
class Scanner;
class Parser;
class Token;

#define PARSER_ENTER 0
#define PARSER_EXIT 1

#define PARSER_MATCH 0
#define PARSER_LOAD 1

using namespace std;

/* The Admin class is responsible for reading source files and writing to output streams.
 * It also performs white space and invisible character filtering on the input stream.
 * It is generally an interface between files and the compiler.
 */
class Admin
{
private:
	int linePos, lineCount;
	bool traceScanner, traceParser, outputAST, almostDone;
	string line, fileName;
	ifstream * source;
	ostream * output, * errOutput;
	Scanner* sc;
	Parser* ps;
        SemanticAnalyzer* sa;
	
	bool isWhiteSpace(char c);
	bool isInvisibleCharacter(char c);
public:
	vector<Token> vec;

	// Constructor/deconstructors/related
	Admin(void);
	Admin(ifstream & file, string fileName, ostream & out);
	Admin(ifstream & file, string fileName, ostream & out, bool traceEnabled);
	Admin(const Admin &other);
	Admin& operator= (const Admin& rhs);
	~Admin(void);

	// Getters/setters
	bool getOutputAST();
	void setOutputAST(bool outputAST);
	void setOutputStream(ostream & out);
	void setErrOutputStream(ostream & out);
	
	// Logging functions
	void scannerLog();
	void scannerLogEnd();
    void parserLog(string functionName, int mode);
	void parserLog(int type, int mode);
	void parserLog(ASTNode * topNode);
    void syntaxError(string expected, int found);
	void cancelAST();
	
	// Other functions
	void compile(int processTo);
	void enableOutputAST();
	char getCh(bool skipWs);
	void endLine();
	void unget();
};

#endif
