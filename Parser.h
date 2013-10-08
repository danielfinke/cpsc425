#ifndef Parser_H
#define Parser_H

#include <string>
#include "Token.h"
#include "ASTNode.h"
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
        Token lookahead;
        
        typedef ASTNode *(Parser::*functionPtr)();
        
        ASTNode * transition(string functionName, functionPtr ptr);
      
        void match(int expected);       
        ASTNode * program();
        ASTNode * declaration();
        ASTNode * nonVoidSpecifier();
        ASTNode * varDecTail();
        ASTNode * varName();
        ASTNode * funDecTail();
        ASTNode * params();
        ASTNode * param();
        ASTNode * statement();
        ASTNode * idStmt();
        ASTNode * assignStmtTail();
        ASTNode * callTail();
        ASTNode * arguments();
        ASTNode * compoundStmt();
        ASTNode * ifStmt();
        ASTNode * loopStmt();
        ASTNode * exitStmt();
        ASTNode * continueStmt();
        ASTNode * returnStmt();
        ASTNode * nullStmt();
        ASTNode * branchStmt();
        ASTNode * caseStmt();
        ASTNode * expression();
        ASTNode * addExp();
        ASTNode * term();
        ASTNode * factor();
        ASTNode * nidFactor();
        ASTNode * idFactor();
        
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
