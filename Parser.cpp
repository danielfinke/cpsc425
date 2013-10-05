#include "Parser.h"
#include "Token.h"
#include "Scanner.h"
#include "Admin.h"
#include <iostream>

Parser::Parser(void) : admin(NULL), sc(NULL), lookahead(0)
{
}
Parser::Parser(Admin& adminMod, Scanner& scanner) : admin(&adminMod), sc(&scanner),lookahead(0) {
}
/* We do not create new instances of Admin or Scanner in either of the copy constructor/assignment operators
 * because there should only be one instance available (these functions should rarely be used)
 */
Parser::Parser(const Parser &other) : admin(other.admin), sc(other.sc),lookahead(other.lookahead) {
}
Parser& Parser::operator= (const Parser &rhs)
{
    // do the copy
    admin = rhs.admin;
	sc = rhs.sc;
        lookahead = rhs.lookahead;
 
    // return the existing object
    return *this;
}

Parser::~Parser(void)
{
	// The Parser does not delete the Scanner or Admin instances, because it does not have ownership.
}

/* Repeatedly request tokens from the scanner until ENDFILE is detected.
 * Does not display commented-out lexemes.
 * Tokens are stored in "vec" for logging purposes.
 */
void Parser::loopScanner() {
	Token tok;
	do {
		tok = sc->getToken();
		if(sc->namesRev[tok.getTokenType()] == "IGNORED") { continue; }

		admin->vec.push_back(tok);
	} while(tok.getTokenType() != sc->ENDFILE);
	admin->logEnd();
}

void Parser::program(){
    
}

void Parser::declaration(){
    
}

void Parser::nonVoidSpecifier(){
    
}

void Parser::decTail(){
    
}

void Parser::varDecTail(){
    
}

void Parser::varName(){
    
}

void Parser::funDecTail(){
    
}

void Parser::params(){
    
}

void Parser::param(){
    
}

void Parser::statement(){
    
}

void Parser::idStmt(){
    
}

void Parser::idStmtTail(){
    
}

void Parser::assignStmtTail(){
    
}

void Parser::callStmtTail(){
    
}

void Parser::callTail(){
    
}

void Parser::arguments(){
    
}

void Parser::compoundStmt(){
    
}

void Parser::ifStmt(){
    
}

void Parser::loopStmt(){
    
}

void Parser::exitStmt(){
    
}

void Parser::continueStmt(){
    
}

void Parser::returnStmt(){
    
}

void Parser::nullStmt(){
    
}

void Parser::branchStmt(){
    
}

void Parser::caseStmt(){
    
    
}

void Parser::expression(){
    
}

void Parser::addExp(){
    
}

void Parser::term(){
    
}

void Parser::factor(){
    
}

void Parser::nidFactor(){
    
}

void Parser::idFactor(){
    
}

void Parser::idTail(){
    
}

void Parser::varTail(){
    
}

void Parser::relOp(){
    
}

void Parser::addOp(){
    
}

void Parser::multOp(){
    
}

void Parser::uMinus(){
   
}

void Parser:: match(int expected){
    
    /*if(lookahead == expected){
     lookahead = Scanner.getToken().name;
     else syntaxError();
     }*/
}