#include "Admin.h"
#include "Scanner.h"
#include "Parser.h"
#include <iostream>

Admin::Admin(void) : linePos(0), lineCount(0), traceScanner(false),
        traceParser(false), outputAST(false), almostDone(false), line(""),
	source(NULL), output(&cout), sc(NULL), ps(NULL)
{
	ASTNode::lookup = NULL;
}

Admin::Admin(ifstream & file, ostream & out) : linePos(0), lineCount(0),
        traceScanner(false), traceParser(false), outputAST(false),
		almostDone(false),
	line(""), source(&file), output(&out),
	sc(new Scanner(*this)), ps(new Parser(*this, *sc))
{
	ASTNode::lookup = sc;
}

Admin::Admin(ifstream & file, ostream & out, bool traceEnabled) : linePos(0), lineCount(0),
        traceScanner(traceEnabled), traceParser(traceEnabled),
		outputAST(true),
	almostDone(false), line(""), source(&file), output(&out),
	sc(new Scanner(*this)), ps(new Parser(*this, *sc))
{
	ASTNode::lookup = sc;
}

Admin::Admin(const Admin &other) : linePos(other.linePos), lineCount(other.lineCount),
        traceScanner(other.traceScanner), traceParser(other.traceParser),
		outputAST(other.outputAST), almostDone(other.almostDone),
        line(other.line), source(other.source), output(other.output), sc(new Scanner(*this)), ps(new Parser(*this, *sc))
{
	ASTNode::lookup = sc;
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
    source = rhs.source;
    output = rhs.output;

    sc = new Scanner(*this);
    ps = new Parser(*this, *sc);
	
	ASTNode::lookup = sc;
 
    // return the existing object
    return *this;
}

Admin::~Admin(void)
{
	vec.clear();
	delete sc;
	delete ps;
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

void Admin::setOutputStream(ostream &out) {
	output = &out;
}

// Compile process is small currently. Only loops through the scanner.
void Admin::compile() {
	//ps->loopScanner();
    ps->startParsing();
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
            *output << lineCount << ": " << line << endl;
    }
}

// Logs scanner output. With trace disabled, only prints errors.
void Admin::scannerLogEnd() {
	for(int i = 0; i < vec.size(); i++) {
		Token tok = vec.at(i);
		if(traceScanner || sc->namesRev[tok.getTokenType()] == "ERROR") {
			*output << "  " << lineCount << ": (" << sc->namesRev[tok.getTokenType()] << ", ";
		}
		
		// If token has values, display them
		if(traceScanner || sc->namesRev[tok.getTokenType()] == "ERROR") {
                    if(tok.getAttributeValue() != -2) {
			*output << tok.getAttributeValue() << ")";
                    }
                    else {
                        *output << "null)";
                    }
		}
		// Display name if token is an identifier
		if(traceScanner && sc->namesRev[tok.getTokenType()] == "ID") {
			*output << " => \"" << sc->getIdentifierName(tok.getAttributeValue()) << "\"";
		}
		// Bump the error counter
		if(sc->namesRev[tok.getTokenType()] == "ERROR") {
			*output << " => \"" << sc->getErrorName(tok.getAttributeValue());
		}
		
		if(traceScanner || sc->namesRev[tok.getTokenType()] == "ERROR") {
			*output << endl;
		}
		
		if(sc->namesRev[tok.getTokenType()] == "ENDFILE") {
			int errors = sc->getErrorCount();
			if(errors > 0) {
				*output << "BUILD FAILED (" << errors << " errors)";
			}
			else {
				*output << "BUILD SUCCEEDED";
			}
			*output << endl;
		}
	}
	
	vec.clear();
}

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
void Admin::parserLog(ASTNode * topNode) {
	if(outputAST) {
		*output << endl;
		topNode->printNode(0, output);
	}
}

// Go back one character in the input line.
void Admin::unget() { linePos--; }

void Admin :: syntaxError(int expected, int found){
	*output << "Line " << lineCount << ": Syntax error. Found " << sc->namesRev[found]
			<< " (expected " << sc->namesRev[expected] << ")" << endl;
	*output << "Terminating..." << endl;
    exit(-1);
}
