#ifndef Parser_H
#define Parser_H

#include <string>
class Admin;
class Scanner;

using namespace std;

/* In its current state, the Parser class solely loops through requests on the Scanner.
 * Each iteration of its main loop in the loopScanner() function produces one single token.
 * These tokens are sent to the Admin instance for reporting purposes, and then subsequently discarded.
 */
class Parser
{
private:
	Admin* admin;
	Scanner* sc;
        int lookahead;
        
        typedef void(Parser::*functionPtr)();
        
        void transition(string functionName, functionPtr ptr);
      
        void match(int expected);       
        void program();
        void declaration();
        void nonVoidSpecifier();
        void decTail();
        void varDecTail();
        void varName();
        void funDecTail();
        void params();
        void param();
        void statement();
        void idStmt();
        void idStmtTail();
        void assignStmtTail();
        void callStmtTail();
        void callTail();
        void arguments();
        void compoundStmt();
        void ifStmt();
        void loopStmt();
        void exitStmt();
        void continueStmt();
        void returnStmt();
        void nullStmt();
        void branchStmt();
        void caseStmt();
        void expression();
        void addExp();
        void term();
        void factor();
        void nidFactor();
        void idFactor();
        void idTail();
        void varTail();
        void relOp();
        void addOp();
        void multOp();
        void uMinus();
        
        bool isStatementLookahead();
        bool isExpressionLookahead();
        bool isRelopLookahead();
        bool isAddopLookahead();
        bool isMultopLookahead();
public:
	// Constructors/deconstructors/related
	Parser(void);
	Parser(Admin& adminMod, Scanner& scanner);
	Parser(const Parser &other);
        Parser& operator= (const Parser &rhs);
	~Parser(void);

	// Main functions
	void loopScanner();
        void startParsing();
        
        
};

#endif
