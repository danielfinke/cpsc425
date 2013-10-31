#ifndef Parser_H
#define Parser_H

#include <string>
#include <vector>
#include "Token.h"
#include "ASTNode.h"
#include "ASTFunctionNode.h"
class Admin;
class Scanner;

using namespace std;

/* In the Scanner phase, the Parser class solely loops through requests on the Scanner.
 * Each iteration of its main loop in the loopScanner() function produces one single token.
 * These tokens are sent to the Admin instance for reporting purposes, and then subsequently discarded.
 */

/*In the Parser Basic phase, the parser uses a top-down, recursive descent parsing
 * style to parse the code. if errors are encountered, the compiler terminates and
 * an error message is printed. It also creates the Abstract syntax tree.
 */
class Parser
{
private:
	int errorCount, loopNesting, compoundCount, compCountOld;
	Admin* admin;
	Scanner* sc;
	Token lookahead;
	ASTNode * astTop;
	ASTFunctionNode * curFunc;

	typedef ASTNode *(Parser::*functionPtr)(vector<int>);

	ASTNode * transition(string functionName, functionPtr ptr, vector<int> syncSet);

	bool match(int expected, vector<int> syncSet);
	ASTNode * program(vector<int> syncSet);
	ASTNode * declaration(vector<int> syncSet);
	ASTNode * nonVoidSpecifier(vector<int> syncSet);
	ASTNode * varDecTail(vector<int> syncSet);
	ASTNode * varName(vector<int> syncSet);
	ASTNode * funDecTail(vector<int> syncSet);
	ASTNode * params(vector<int> syncSet);
	ASTNode * param(vector<int> syncSet);
	ASTNode * statement(vector<int> syncSet);
	ASTNode * idStmt(vector<int> syncSet);
	ASTNode * assignStmtTail(vector<int> syncSet);
	ASTNode * callTail(vector<int> syncSet);
	ASTNode * arguments(vector<int> syncSet);
	ASTNode * compoundStmt(vector<int> syncSet);
	ASTNode * ifStmt(vector<int> syncSet);
	ASTNode * loopStmt(vector<int> syncSet);
	ASTNode * exitStmt(vector<int> syncSet);
	ASTNode * continueStmt(vector<int> syncSet);
	ASTNode * returnStmt(vector<int> syncSet);
	ASTNode * nullStmt(vector<int> syncSet);
	ASTNode * branchStmt(vector<int> syncSet);
	ASTNode * caseStmt(vector<int> syncSet);
	ASTNode * expression(vector<int> syncSet);
	ASTNode * addExp(vector<int> syncSet);
	ASTNode * term(vector<int> syncSet);
	ASTNode * factor(vector<int> syncSet);
	ASTNode * nidFactor(vector<int> syncSet);
	ASTNode * idFactor(vector<int> syncSet);

	bool isStatementLookahead();
	bool isExpressionLookahead();
	bool isRelopLookahead();
	bool isAddopLookahead();
	bool isMultopLookahead();
	
	//void syntaxCheck(vector<int> syncSet);
	void syntaxError(int expected, vector<int> syncSet);
	void syntaxError(string expected, vector<int> syncSet);
        
public:
	// Constructors/deconstructors/related
	Parser(void);
	Parser(Admin& adminMod, Scanner& scanner);
	Parser(const Parser &other);
        Parser& operator= (const Parser &rhs);
	~Parser(void);

	// Main functions
	void loopScanner();
    ASTNode * startParsing();
	
	// Getters/setters
	int getErrorCount();
};

#endif
