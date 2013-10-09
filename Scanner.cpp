#include "Scanner.h"
#include "Admin.h"
#include <iostream>

// The friendly names for token integer enums
string Scanner::namesRev[50] = {"ID", "NUM", "BLIT", "ENDFILE", "ERROR",
			"AND", "BOOL", "BRANCH", "CASE", "CONTINUE", "DEFAULT", "ELSE", "END",
			"EXIT", "IF", "INT", "LOOP", "MOD", "NOT", "OR", "REF", "RETURN", "VOID",
			"PLUS", "MINUS", "MULT", "DIV", "ANDTHEN", "ORELSE", "LT", "LTEQ",
			"GT", "GTEQ", "EQ", "NEQ", "ASSIGN", "SEMI", "COMMA", "LPAREN", "RPAREN",
			"LSQR", "RSQR", "LCRLY", "RCRLY",
			"INLINECOMMENT", "COMMENTOPEN", "COMMENTCLOSE",
			"IGNORED", "COLON","NULLSTMT"};

Scanner::Scanner() : symbolCount(0), commentDepth(0), errorCount(0), wordTable(unordered_map<string, pair<int, int>>()),
	symbolTable(unordered_map<string, pair<int, int>>())
{
	populateWordTable();
}
Scanner::Scanner(Admin& adminMod) : symbolCount(0), commentDepth(0), errorCount(0), wordTable(unordered_map<string, pair<int, int>>()),
	symbolTable(unordered_map<string, pair<int, int>>()), admin(&adminMod)
{
	populateWordTable();
}
/* We do not create new instances of Admin or Parser in either of the copy constructor/assignment operators
 * because there should only be one instance available (these functions should rarely be used)
 */
Scanner::Scanner(const Scanner &other) : symbolCount(other.symbolCount), commentDepth(other.commentDepth),
	errorCount(other.errorCount), wordTable(other.wordTable), symbolTable(other.symbolTable), admin(other.admin)
{
}
Scanner& Scanner::operator= (const Scanner &rhs)
{
    // do the copy
    symbolCount = rhs.symbolCount;
	commentDepth = rhs.commentDepth;
	errorCount = rhs.errorCount;
	wordTable = rhs.wordTable;
	symbolTable = rhs.symbolTable;
	admin = rhs.admin;
 
    // return the existing object
    return *this;
}

Scanner::~Scanner() {
	// The Parser does not delete the Scanner or Admin instances, because it does not have ownership.

	wordTable.clear();
	symbolTable.clear();
	spellingTable.clear();
	errorTable.clear();
}

// Returns true if the character is a letter from A-Z (case insensitive)
bool Scanner::isLetter(char c) {
	return (c >= 65 && c <= 90) ||
		(c >= 97 && c <= 122);
}

// Returns true if the character is between 0-9
bool Scanner::isDigit(char c) {
	return c >= 48 && c <= 57;
}

/* Returns true if the character is acceptable as part of an identifier (not including first char!)
 * The criteria is:
 * ID = letter (letter | underscore | dollarsign | digit)*
 */
bool Scanner::isValidIdChar(char c) {
	return isLetter(c) || isDigit(c) || c == '$' || c == '_';
}

// Starts the fetch of a new lexeme. Breaks down into letters, numbers, and symbols
pair<string, int> Scanner::getLexeme() {
	char c;
	
	// Cycle through invisible characters out here first
	while(c = admin->getCh(true)) {
		if(isLetter(c)) {
			/* The value returned by this pair is irrelevant for identifiers
			 * because they have not yet been assigned a unique index in the
			 * symbol table
			 */
			return make_pair(getIdentifier(c), -4);
		}
		else if(isDigit(c)) {
			return make_pair("NUM", getNumeral(c));
		}
		else {
			int symbol = getSpecial(c);
			if(c == ERROR) {
				return make_pair(string(&c), ERROR);
			}
			return make_pair("SYMBOL", symbol);
		}
	}

	return make_pair("SYMBOL", ENDFILE);
}

// Continues concatenating characters to an identifier string until it is not valid, then removes the recent one
string Scanner::getIdentifier(char c) {
	string ident = "";
	ident += c;
	
	while(c = admin->getCh(false)) {
		if(!isValidIdChar(c)) {
			admin->unget();
			break;
		}

		ident += c;
	}

	return ident;
}

// Continues concatenating characters to a number until a non-number is found
int Scanner::getNumeral(char c) {
	string numeral = "";
	numeral += c;
	
	while(c = admin->getCh(false)) {
		if(!isDigit(c)) {
			admin->unget();
			break;
		}

		numeral += c;
	}

	return atoi(numeral.c_str());
}

/* DFA state when a symbol has been initially detected.
 * getSpecial() breaks down into multiple other states depending on the symbol found.
 * Comment-related states use the commentDepth integer to determine the level of comment nesting
 */
int Scanner::getSpecial(char c) {
	switch(c) {
	// First bunch are for trivial cases
	case '+':
		return PLUS;
	case '=':
		return EQ;
	case ';':
		return SEMI;
	case ',':
		return COMMA;
	case '(':
		return LPAREN;
	case ')':
		return RPAREN;
	case '[':
		return LSQR;
	case ']':
		return RSQR;
	case '{':
		return LCRLY;
	case '}':
		return RCRLY;
	case '-':
		c = admin->getCh(false);
		if(c != '-') {
			admin->unget();
			return MINUS;
		}
		else {
			return INLINECOMMENT;
		}
		break;
	case '*':
		if(commentDepth == 0) {
			return MULT;
		}
		else {
			c = admin->getCh(false);
			if(c != '/') {
				admin->unget();
				return MULT;
			}
			else {
				commentDepth--;
				return COMMENTCLOSE;
			}
		}
		break;
	case '/':
		c = admin->getCh(false);
		if(c == '=') {
			return NEQ;
		}
		else if(c == '*') {
			commentDepth++;
			return COMMENTOPEN;
		}
		else {
			admin->unget();
			return DIV;
		}
		break;
	case '&':
		c = admin->getCh(false);
		if(c == '&') {
			return ANDTHEN;
		}
		else {
			admin->unget();
			return ERROR;
		}
		break;
	case '|':
		c = admin->getCh(false);
		if(c == '|') {
			return ORELSE;
		}
		else {
			admin->unget();
			return ERROR;
		}
		break;
	case '<':
		c = admin->getCh(false);
		if(c == '=') {
			return LTEQ;
		}
		else {
			admin->unget();
			return LT;
		}
		break;
	case '>':
		c = admin->getCh(false);
		if(c == '=') {
			return GTEQ;
		}
		else {
			admin->unget();
			return GT;
		}
		break;
	case ':':
		c = admin->getCh(false);
		if(c == '=') {
			return ASSIGN;
		}
		else {
			return COLON;
		}
		break;
	case EOF:
		return ENDFILE;
		break;
	default:
		return ERROR;
	}
}

// Fills out the word table with reserved keywords and their values
void Scanner::populateWordTable() {
	wordTable.insert(make_pair("and", make_pair(AND, -2)));
	wordTable.insert(make_pair("branch", make_pair(BRANCH, -2)));
	wordTable.insert(make_pair("bool", make_pair(BOOL, -2)));
	wordTable.insert(make_pair("case", make_pair(CASE, -2)));
	wordTable.insert(make_pair("continue", make_pair(CONTINUE, -2)));
	wordTable.insert(make_pair("default", make_pair(DEFAULT, -2)));
	wordTable.insert(make_pair("else", make_pair(ELSE, -2)));
	wordTable.insert(make_pair("end", make_pair(END, -2)));
	wordTable.insert(make_pair("exit", make_pair(EXIT, -2)));
	wordTable.insert(make_pair("if", make_pair(IF, -2)));
	wordTable.insert(make_pair("int", make_pair(INT, -2)));
	wordTable.insert(make_pair("loop", make_pair(LOOP, -2)));
	wordTable.insert(make_pair("mod", make_pair(MOD, -2)));
	wordTable.insert(make_pair("not", make_pair(NOT, -2)));
	wordTable.insert(make_pair("or", make_pair(OR, -2)));
	wordTable.insert(make_pair("ref", make_pair(REF, -2)));
	wordTable.insert(make_pair("return", make_pair(RETURN, -2)));
	wordTable.insert(make_pair("void", make_pair(VOID, -2)));
	wordTable.insert(make_pair("true", make_pair(BLIT, 1)));
	wordTable.insert(make_pair("false", make_pair(BLIT, 0)));
}

int Scanner::getErrorCount() { return errorCount; }

// Wraps getToken(pair<string, int> tok)
Token Scanner::getToken() {
	Token nonIgnoredToken;
	do {
		nonIgnoredToken = getToken(getLexeme());
	} while(nonIgnoredToken.getTokenType() == IGNORED);
	return nonIgnoredToken;
}

// Builds an appropriate token for a given lexeme and returns it
Token Scanner::getToken(pair<string, int> tok) {
	Token token;

	// These are within comment scope and are discarded
	if(commentDepth > 0 || tok.second == COMMENTCLOSE) {
		if(tok.second == ENDFILE && tok.first == "SYMBOL") {
			token = Token(tok.second, -2);
		}
		else {
			token = Token(IGNORED, -2);
		}
	}
	// Inline comments trigger an instant endline in the source file
	else if(tok.second == INLINECOMMENT) {
		token = Token(IGNORED, -2);
		admin->endLine();
	}
	else if(tok.first == "NUM") {
		token = Token(NUM, tok.second);
	}
	else if(tok.first == "SYMBOL") {
		token = Token(tok.second, -2);
	}
	else {
		// Save the character if it was an error token
		if(tok.second == ERROR) {
			token = Token(ERROR, errorCount++);
			errorTable.push_back(tok.first);
		}
	
		// Check if the word is a reserved keyword
		else if(wordTable.count(tok.first) > 0) {
			token = Token(wordTable.at(tok.first).first, wordTable.at(tok.first).second);
		}
		// If not, it is an identifier
		// First, check if it's already got an id in the symbol table
		else if(symbolTable.count(tok.first) == 0) {
			symbolTable.insert(make_pair(tok.first, make_pair(ID, symbolCount)));
			// symbolCount is unique count of identifiers
			token = Token(ID, symbolCount++);
			// Add a vector entry for looking up the identifier's friendly name from the id in the symbol table
			spellingTable.push_back(tok.first);
		}
		// Identifier was already added, just look up its id
		else {
			token = Token(symbolTable.at(tok.first).first, symbolTable.at(tok.first).second);
		}
	}

	return token;
}

// Returns the friendly name of an identifier, given its id
string Scanner::getIdentifierName(int id) {
	return spellingTable.at(id);
}

// Returns the string that caused an error
string Scanner::getErrorName(int id) {
	return errorTable.at(id);
}
