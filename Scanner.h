#ifndef Scanner_H
#define Scanner_H

#include <unordered_map>
#include <vector>
#include "Token.h"
class Admin;

// -3 is placeholder for attribute value for identifiers in wordtable
// -4 is placeholder in getLexeme result for identifiers

using namespace std;

/* The Scanner class is a DFA implementation for parsing out tokens from the source file.
 * Token parsing drills down into IDs, NUMs, and SYMBOLs to start.
 * SYMBOLs use a small component of the DFA as well as a counter to determine the exact
 * lexeme supplied in the source code.
 *
 * The scanner also deals with internal integer representations of identifiers;
 * that is, it can translate "x" -> <ID, 2> -> "x". This serves the logging module by providing
 * friendly names for ID tokens and enables an array representation of IDs internally.
 *
 * The admin->unget() function is used heavily in this class; it is necessary to read the maximum
 * substring until the lexeme is no longer valid, and then undo the last character concat.
 */
class Scanner {
	private:
		int commentDepth, symbolCount, errorCount;
		unordered_map<string, pair<int, int>> wordTable;
		unordered_map<string, pair<int, int>> symbolTable;
		vector<string> spellingTable, errorTable;
		Admin * admin;

		// Condition functions
		bool isLetter(char c);
		bool isDigit(char c);
		bool isValidIdChar(char c);

		// DFA-related
		pair<string, int> getLexeme();
		string getIdentifier(char c);
		int getNumeral(char c);
		int getSpecial(char c);

		// Miscellaneous functions
		Token getToken(pair<string, int> tok);
		void populateSymbolsReadWrite();
		void populateWordTable();
	public:
		// All available token types
		enum tokenNames {ID, NUM, BLIT, ENDFILE, ERROR,
			AND, BOOL, BRANCH, CASE, CONTINUE, DEFAULT, ELSE, END,
			EXIT, IF, INT, LOOP, MOD, NOT, OR, REF, RETURN, VOID,
			PLUS, MINUS, MULT, DIV, ANDTHEN, ORELSE, LT, LTEQ,
			GT, GTEQ, EQ, NEQ, ASSIGN, SEMI, COMMA, LPAREN, RPAREN,
			LSQR, RSQR, LCRLY, RCRLY,
			INLINECOMMENT, COMMENTOPEN, COMMENTCLOSE,
			IGNORED, COLON,NULLSTMT};
		// String names of token types
		static string namesRev[50];

		// Constructors/deconstructors/related
		Scanner();
		Scanner(Admin& adminMod);
		Scanner(const Scanner &other);
		Scanner& operator= (const Scanner &rhs);
		~Scanner();

		// Main functions
		int getErrorCount();
		int getIdentifierCount();
		Token getToken();
		string getIdentifierName(int id);
		string getErrorName(int id);
};

#endif
