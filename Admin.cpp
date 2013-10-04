#include "Admin.h"
#include "Scanner.h"
#include "Parser.h"
#include <iostream>

Admin::Admin(void) : linePos(0), lineCount(0), trace(false), almostDone(false), line(""),
	source(NULL), output(&cout)
{
}

Admin::Admin(ifstream & file, ostream & out) : linePos(0), lineCount(0), trace(false), almostDone(false),
	line(""), source(&file), output(&out),
	sc(new Scanner(*this)), ps(new Parser(*this, *sc)) {
}

Admin::Admin(ifstream & file, ostream & out, bool traceEnabled) : linePos(0), lineCount(0), trace(traceEnabled),
	almostDone(false), line(""), source(&file), output(&out),
	sc(new Scanner(*this)), ps(new Parser(*this, *sc)) {
}

Admin::Admin(const Admin &other) : linePos(other.linePos), lineCount(other.lineCount), trace(other.trace), almostDone(false),
        line(other.line), source(other.source), output(other.output), sc(new Scanner(*this)), ps(new Parser(*this, *sc))
{
}

Admin& Admin::operator= (const Admin &rhs)
{
    // do the copy
    linePos = rhs.linePos;
	lineCount = rhs.lineCount;
	trace = rhs.trace;
	almostDone = rhs.almostDone;
	line = rhs.line;
        source = rhs.source;
        output = rhs.output;

	sc = new Scanner(*this);
	ps = new Parser(*this, *sc);
 
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
	ps->loopScanner();
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
	if(lineCount > 0) { log(); }
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
}

// Logs scanner output. With trace disabled, only prints errors.
void Admin::log() {
	if(trace) {
		*output << lineCount << ": " << line << endl;
	}
	logEnd();
}

void Admin::logEnd() {
	for(int i = 0; i < vec.size(); i++) {
		Token tok = vec.at(i);
		if(trace || sc->namesRev[tok.getTokenType()] == "ERROR") {
			*output << "  " << lineCount << ": (" << sc->namesRev[tok.getTokenType()] << ", ";
		}
		
		// If token has values, display them
		if((trace || sc->namesRev[tok.getTokenType()] == "ERROR") && tok.getAttributeValue() != -2) {
			*output << tok.getAttributeValue() << ")";
		}
		else {
			*output << "null)";
		}
		// Display name if token is an identifier
		if(trace && sc->namesRev[tok.getTokenType()] == "ID") {
			*output << " => \"" << sc->getIdentifierName(tok.getAttributeValue()) << "\"";
		}
		// Bump the error counter
		if(sc->namesRev[tok.getTokenType()] == "ERROR") {
			*output << " => \"" << sc->getErrorName(tok.getAttributeValue());
		}
		
		if(trace || sc->namesRev[tok.getTokenType()] == "ERROR") {
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

// Go back one character in the input line.
void Admin::unget() { linePos--; }
