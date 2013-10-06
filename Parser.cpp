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
    
    do{
         declaration();
    }while(lookahead == sc->INT || lookahead == sc->BOOL || lookahead == sc->VOID);
    
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
    switch(lookahead){
        case sc->ID:
            idStmt();
            break;
        case sc->LCRLY:
            idStmt();
            break;
        case sc->IF:
            ifStmt();
            break;
        case sc->LOOP:
            loopStmt();
            break;
        case sc->EXIT:
            exit();
            break;
        case sc->CONTINUE:
            continueStmt();
            break;
        case sc->RETURN:
            returnStmt();
            break;
        case sc->BRANCH:
            branchStmt();
            break;
        default:
            nullStmt();
            break;
    }
    
}

void Parser::idStmt(){
    match(sc->ID);
    idStmtTail();
}

void Parser::idStmtTail(){
    if(lookahead == sc->LSQR || lookahead == sc->ASSIGN){
        assignStmtTail();
    }
    else{
        callStmtTail();
    }
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
    addExp();
    if(isRelopLookahead())
    {
        relOp();
        addExp();
    }
}

void Parser::addExp(){
    if(lookahead == sc->MINUS){
        uMinus();
    }
    term();
    while(isAddopLookahead()){
        addOp();
        term();
    }
}

void Parser::term(){
    factor();
    while(isMultopLookahead()){
        multOp();
        factor();
    }
}

void Parser::factor(){
    if(lookahead == sc->ID){
        idFactor();
    }
    else{
        nidFactor();
    }
}

void Parser::nidFactor(){
    switch(lookahead){
        case sc->NOT:
            match(sc->NOT);
            factor();
            break;
        case sc->LPAREN:
            match(sc->LPAREN);
            expression();
            match(sc->RPAREN);
            break;
        case sc->NUM:
            match(sc->NUM);
            break;
        default:
            match(sc->BLIT);
            break;        
    }
}

void Parser::idFactor(){
    match(sc->ID);
    idTail();
}

void Parser::idTail(){
    if(lookahead == sc->LPAREN){
        callTail();
    }
    else{
        varTail();
    }
}

void Parser::varTail(){
    if(lookahead == sc->LSQR){
        match(sc->LSQR);
        addExp();
        match(sc->RSQR);
    }
}

void Parser::relOp(){
    switch(lookahead){
        case sc->LTEQ:
            match(sc->LTEQ);
            break;
        case sc->LT:
            match(sc->LT);
            break;
        case sc->GT:
            match(sc->GT);
            break;
        case sc->GTEQ:
            match(sc->GTEQ);
            break;
        case sc->EQ:
            match(sc->EQ);
            break;
        default:
            match(sc->NEQ);
            break;
    }
}

void Parser::addOp(){
    switch(lookahead){
        case sc->PLUS:
            match(sc->PLUS);
            break;
        case sc->MINUS:
            match(sc->MINUS);
            break;
        case sc-> OR:
            match(sc->OR);
            break;
        //default == ORELSE
        default:
            match(sc->ORELSE);
            break;
    }
}

void Parser::multOp(){
    switch(lookahead){
        case sc->MULT:
            match(sc->MULT);
            break;
        case sc->DIV:
            match(sc->DIV);
            break;
        case sc->MOD:
            match(sc->MOD);
            break;
        case sc->AND:
            match(sc->AND);
            break;
        //default == ANDTHEN
        default:
            match(sc->ANDTHEN);
            break;
    }
}

void Parser::uMinus(){
    match(sc->MINUS);
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

bool Parser:: isRelopLookahead(){
    switch(lookahead){
        case sc->LTEQ:
        case sc->LT:
        case sc->GT:
        case sc->GTEQ:
        case sc->EQ:
        case sc->NEQ:return true;
    }
    return false;
}

bool Parser:: isAddopLookahead(){
    switch(lookahead){
        case sc->PLUS:
        case sc->MINUS:
        case sc->OR:
        case sc->ORELSE:return true;
    }
    return false;
}

bool Parser:: isMultopLookahead(){
    switch(lookahead){
        case sc->MULT:
        case sc->DIV:
        case sc->MOD:
        case sc->AND:
        case sc->ANDTHEN:return true;
    }
    return false;
}