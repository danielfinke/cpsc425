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

void Parser::startParsing(){
    lookahead = sc->getToken().getTokenType();
    program();
}

void Parser::program(){
    
    declaration();
    
}

void Parser::declaration(){
    if(lookahead == sc ->VOID){
        match(sc->VOID);
        match(sc->ID);
        funDecTail();     
    }
    else {
        nonVoidSpecifier();
        match(sc->ID);
        decTail();
    }
}

void Parser::nonVoidSpecifier(){
    if(lookahead = sc->INT)
        match(sc->INT);
    else
        match(sc->BOOL);
    
}

void Parser::decTail(){
    varDecTail();
    funDecTail();
}

void Parser::varDecTail(){
    if(lookahead == sc->LSQR){
        match(sc->LSQR);
        addExp();
        match(sc->RSQR);
    }
    while(lookahead == sc->COMMA){
        match(sc->COMMA);
        varName();
    }
    
    match(sc->SEMI);
}

void Parser::varName(){
    match(sc->ID);
    
    if(lookahead == sc->LSQR){
        match(sc->LSQR);
        addExp();
        match(sc->RSQR);
    }
}

void Parser::funDecTail(){
    match(sc->LPAREN);
    params();
    match(sc->RPAREN);
    compoundStmt();
}

void Parser::params(){
    if(lookahead == sc->REF){
        param();
        while(lookahead == sc->COMMA){
            match(sc->COMMA);
            param();
        }
    }
    else{
        match(sc->VOID);
    }
    
}

void Parser::param(){
    if(lookahead == sc->REF){
        match(sc->REF);
        nonVoidSpecifier();
        match(sc->ID);
    }
    else{
        nonVoidSpecifier();
        match(sc->ID);
        
        if(lookahead == sc->LSQR){
            match(sc->LSQR);
            match(sc->RSQR);
        }
    }
}

void Parser::statement(){
    //DO ME
    
}

void Parser::idStmt(){
    match(sc->ID);
    idStmtTail();
}

void Parser::idStmtTail(){
    //DO ME
}

void Parser::assignStmtTail(){
    if(lookahead == sc->LSQR){
        match(sc->LSQR);
        addExp();
        match(sc->RSQR);
    }
    
    match(sc->ASSIGN);
    expression();
    match(sc->SEMI);
}

void Parser::callStmtTail(){
    callTail();
    match(sc->SEMI);
    
}

void Parser::callTail(){
    match(sc->LPAREN);
        
    if(isExpressionLookahead()){
        arguments();
    }
    
    match(sc->RPAREN);
    
}

void Parser::arguments(){
    expression();
    while(lookahead == sc->COMMA){
        match(sc->COMMA);
        expression();
    }
}

void Parser::compoundStmt(){
    match(sc->LCRLY);
    while(lookahead == sc->INT || lookahead == sc->BOOL){
        nonVoidSpecifier();
        match(sc->ID);
        varDecTail();
    }
    do{
        statement();
    }while(isStatementLookahead());
     
    match(sc->RCRLY);
}

void Parser::ifStmt(){
    //DO ME (disambiguation rule)
    match(sc->IF);
    match(sc->LPAREN);
    expression();
    match(sc->RPAREN);
    statement();
    
    if(lookahead == sc->ELSE){
        match(sc->ELSE);
        statement();
    }
}

void Parser::loopStmt(){
    match(sc->LOOP);
    do{
        statement();
    }while(isStatementLookahead());
    match(sc->END);
    match(sc->SEMI);
}

void Parser::exitStmt(){
    match(sc->EXIT);
    match(sc->SEMI);
}

void Parser::continueStmt(){
    match(sc->CONTINUE);
    match(sc->SEMI);
}

void Parser::returnStmt(){
    match(sc->RETURN);
    if(isExpressionLookahead()){
        expression();
    }
    match(sc->SEMI);
}

void Parser::nullStmt(){
    match(sc->SEMI);
}

void Parser::branchStmt(){
    match(sc->BRANCH);
    match(sc->LPAREN);
    addExp();
    match(sc->RPAREN);
    
    do{
        caseStmt();
    }while(lookahead == sc->CASE || lookahead == sc->DEFAULT);
    
    match(sc->END);
    match(sc->SEMI);
        
}

//NOTE colon doesn't exist in language???
void Parser::caseStmt(){
    if(lookahead == sc->CASE){
        match(sc->CASE);
        match(sc->NUM);
       // match(sc->COLON);
        statement();
    }
    else{
        match(sc->DEFAULT);
        //match(sc->COLON);
        statement();
    }
    
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
    
    if(lookahead == expected){
        lookahead = sc->getToken().getTokenType();
    }
    else{
        //syntaxError();
    }
}

bool Parser:: isStatementLookahead(){
   
    switch(lookahead){
        case sc->LCRLY:
        case sc->IF:
        case sc->LOOP:
        case sc-> EXIT:
        case sc->CONTINUE:
        case sc->RETURN:
        case sc->SEMI:
        case sc->ID:
        case sc->BRANCH:return true;   
    }
    return false;
}

//is same for Arguments; Expression; addExp;
bool Parser:: isExpressionLookahead(){
    switch(lookahead){
        case sc->MINUS:
        case sc->NOT:
        case sc->LPAREN:
        case sc->NUM:
        case sc->BLIT:
        case sc->ID:return true;          
    }
    return false;
}