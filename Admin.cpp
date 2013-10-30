#include "Admin.h"
#include "Scanner.h"
#include "Parser.h"
#include <iostream>

Admin::Admin(void) : linePos(0), lineCount(0), traceScanner(false),
        traceParser(false), outputAST(false), almostDone(false), line(""),
		fileName(""),
	source(NULL), output(&cout), errOutput(&cout), sc(NULL), ps(NULL), sa(NULL)
{
	ASTNode::lookup = NULL;
	ASTNode::sa = NULL;
}

Admin::Admin(ifstream & file, string fileName, ostream & out) : linePos(0), lineCount(0),
        traceScanner(false), traceParser(false), outputAST(false),
		almostDone(false), fileName(fileName),
	line(""), source(&file), output(&out), errOutput(&out),
	sc(new Scanner(*this)), ps(new Parser(*this, *sc)), sa(new SemanticAnalyzer(this))
{
	ASTNode::lookup = sc;
	ASTNode::sa = sa;
}

Admin::Admin(ifstream & file, string fileName, ostream & out, bool traceEnabled) : linePos(0), lineCount(0),
        traceScanner(traceEnabled), traceParser(traceEnabled),
		outputAST(false),
	almostDone(false), line(""), fileName(fileName), source(&file), output(&out),
	errOutput(&out), sc(new Scanner(*this)), ps(new Parser(*this, *sc)), sa(new SemanticAnalyzer(this))
{
	ASTNode::lookup = sc;
	ASTNode::sa = sa;
}

Admin::Admin(const Admin &other) : linePos(other.linePos), lineCount(other.lineCount),
        traceScanner(other.traceScanner), traceParser(other.traceParser),
		outputAST(other.outputAST), almostDone(other.almostDone), fileName(other.fileName),
        line(other.line), source(other.source), output(other.output), errOutput(other.errOutput),
		sc(new Scanner(*this)), ps(new Parser(*this, *sc)), sa(new SemanticAnalyzer(this))
{
	ASTNode::lookup = sc;
	ASTNode::sa = sa;
}

Admin& Admin::operator= (const Admin &rhs)
{
    // do the copy
    linePos = rhs.linePos;
    lineCount = rhs.lineCount;
    traceScanner = rhs.traceScanner;
    traceParser = rhs.traceParser;
	outputAST = rhs.outputAST;
    almostDone = rhs.almostDone;
    line = rhs.line;
	fileName = rhs.fileName;
    source = rhs.source;
    output = rhs.output;
	errOutput = rhs.errOutput;

    sc = new Scanner(*this);
    ps = new Parser(*this, *sc);
	sa = new SemanticAnalyzer(this);
	
	ASTNode::lookup = sc;
	ASTNode::sa = sa;
 
    // return the existing object
    return *this;
}

Admin::~Admin(void)
{
	vec.clear();
	delete sc;
	delete ps;
	delete sa;
}

// Returns true if a character is a whitespace character
bool Admin::isWhiteSpace(char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

// Returns true if a character is an invisible character
bool Admin::isInvisibleCharacter(char c) {
	return (c >= 0 && c < 32) || c == 127 || c == 129 || c == 141 || c == 143
		|| c == 144 || c == 157;
}

// Returns true if the output AST is designated to be displayed
bool Admin::getOutputAST() {
	return outputAST;
}

int Admin:: getLineNumber() {
    return lineCount;
}

void Admin::setOutputAST(bool outputAST) {
	this->outputAST = outputAST;
}
void Admin::setOutputStream(ostream &out) {
	output = &out;
}
void Admin::setErrOutputStream(ostream &out) {
	errOutput = &out;
}

// Process up to the desired point as specified by processTo
void Admin::compile(int processTo) {
	ASTNode * top = NULL;
	
	switch(processTo) {
		case 1:
			ps->loopScanner();
			break;
		case 2:
			ps->startParsing();
			break;
		case 3:
			top = ps->startParsing();
			if(top != NULL)
			  {
				   sa->semAnalyze(top);

			  }
			break;
		default:
			// Will be changed as phases get added
			top = ps->startParsing();
			if(top != NULL)
			  {
				   sa->semAnalyze(top);

			  }
			int t = 4;
                         
	}
}

void Admin::enableOutputAST() {
	outputAST = true;
}

// Interfaces with the input file. Reads a whole line, then passes out characters as needed until end of line.
char Admin::getCh(bool skipWs) {
	char c;

    // Load another line if finished the current one
	while(linePos >= line.length()) {
		if(almostDone) {
			return EOF;
		}

		if(!skipWs) {
            linePos++;
			return '\n';
		}
		
		endLine();
	}

	c = line[linePos];

	// Skip whitespace if required by the function call
	while((skipWs && isWhiteSpace(c)) || isInvisibleCharacter(c)) {
		linePos++;
		while(linePos >= line.length()) {
			if(almostDone) {
				return EOF;
			}

			if(!skipWs) {
				linePos++;
				return '\n';
			}

			endLine();
		}

		c = line[linePos];
	}

	linePos++;
	return c;
}

// Refreshes the input line. Also logs the previous one.
void Admin::endLine() {
	if(!source->eof()) {
		linePos = 0;
		lineCount++;
		getline(*source, line);
	}
	else {
		linePos = 0;
		line = "" + EOF;
		almostDone = true;
	}
	if(lineCount > 0 && !almostDone) { scannerLog(); }
}

// Logs scanner input.
void Admin::scannerLog() {
    if(traceScanner) {
		int startIndex = 0;
		
		if(fileName != "") {
			*output << fileName << ":";
		}
		*output << lineCount << ": ";
		
		while(isWhiteSpace(line[startIndex])) {
			startIndex++;
		}
		
		for(int i = startIndex; i < line.length(); i++) {
			*output << line[i];
		}
        *output << endl;
    }
}

// Logs scanner output. With trace disabled, only prints errors.
void Admin::scannerLogEnd() {
	for(int i = 0; i < vec.size(); i++) {
		Token tok = vec.at(i);
		if(traceScanner && sc->namesRev[tok.getTokenType()] != "ERROR") {
			*output << "  " << lineCount << ": (" << sc->namesRev[tok.getTokenType()] << ", ";
		}
		if(sc->namesRev[tok.getTokenType()] == "ERROR") {
			*errOutput << "  " << lineCount << ": (" << sc->namesRev[tok.getTokenType()] << ", ";
		}
		
		// If token has values, display them
		if(traceScanner && sc->namesRev[tok.getTokenType()] != "ERROR") {
			if(tok.getAttributeValue() != -2) {
				*output << tok.getAttributeValue() << ")";
			}
			else {
				*output << "null)";
			}
		}
		if(sc->namesRev[tok.getTokenType()] == "ERROR") {
			if(tok.getAttributeValue() != -2) {
				*errOutput << tok.getAttributeValue() << ")";
			}
			else {
				*errOutput << "null)";
			}
		}
		// Display name if token is an identifier
		if(traceScanner && sc->namesRev[tok.getTokenType()] == "ID") {
			*output << " => \"" << sc->getIdentifierName(tok.getAttributeValue()) << "\"";
		}
		// Bump the error counter
		if(sc->namesRev[tok.getTokenType()] == "ERROR") {
			*errOutput << " => \"" << sc->getErrorName(tok.getAttributeValue()) << "\"";
		}
		
		if(traceScanner && sc->namesRev[tok.getTokenType()] != "ERROR") {
			*output << endl;
		}
		if(sc->namesRev[tok.getTokenType()] == "ERROR") {
			*errOutput << endl;
		}
		
		// No longer used
		/*if(sc->namesRev[tok.getTokenType()] == "ENDFILE") {
			int errors = sc->getErrorCount();
			if(errors > 0) {
				*output << "BUILD FAILED (" << errors << " errors)";
			}
			else {
				*output << "BUILD SUCCEEDED";
			}
			*output << endl;
		}*/
	}
	
	vec.clear();
}

// Log entry/exit of derivation tree functions
void Admin::parserLog(string functionName, int mode) {
	if(traceParser) {
		if(mode == PARSER_ENTER) {
			*output << "Entering " << functionName << endl;
		}
		else if(mode == PARSER_EXIT) {
			*output << "Leaving " << functionName << endl;
		}
	}
}
// Log matching/loading of tokens by parser
void Admin::parserLog(int type, int mode) {
	if(traceParser) {
		if(mode == PARSER_MATCH) {
			*output << "Matched " << sc->namesRev[type] << endl;
		}
		else if(mode == PARSER_LOAD) {
			*output << "Loaded " << sc->namesRev[type] << endl;
		}
	}
}
// Recursively print the abstract syntax tree (AST) produced by the parser
void Admin::parserLog(ASTNode * topNode) {
	if(outputAST) {
		*output << endl;
		topNode->printNode(0, output);
	}
}

// Called if the AST was not built due to syntax errors
void Admin::cancelAST() {
	*output << endl;
	*output << "One or more syntax errors detected. AST could not be built" << endl;
}

// Go back one character in the input line.
void Admin::unget() { linePos--; }

string Admin::getIdentifierName(int id) {
	return sc->getIdentifierName(id);
}

/* syntaxError - prints syntax error information. In the basic parser,
 *				this results in the parser terminating abruptly
 * @param expected		token type expected to be found at current parse locn
 * @param found			token type found at current parse locn
 */
void Admin::syntaxError(string expected, int found){
	if(fileName != "") {
		*errOutput << fileName << ":";
	}
	*errOutput << lineCount << ": Syntax error. Found " << sc->namesRev[found]
			<< " (expected " << expected << ")" << endl;
}

void Admin::semanticError(string desc, int lineNumber) {
	if(fileName != "") {
		*errOutput << fileName << ":";
	}
	*errOutput << lineNumber << ": Semantic error. " << desc << endl;
	sa->incError();
}
