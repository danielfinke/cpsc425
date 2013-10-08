#include "Parser.h"
#include "Token.h"
#include "Scanner.h"
#include "Admin.h"
#include "ASTLoopNode.h"
#include "ASTFunctionCallNode.h"
#include "ASTAssignmentNode.h"
#include "ASTFunctionNode.h"
#include "ASTIfNode.h"
#include "ASTUnaryNode.h"
#include "ASTBinaryNode.h"
#include "ASTLiteralNode.h"
#include "ASTVariableNode.h"
#include "ASTMarkerNode.h"
#include <iostream>

Parser::Parser(void) : admin(NULL), sc(NULL), lookahead(Token())
{
}
Parser::Parser(Admin& adminMod, Scanner& scanner) : admin(&adminMod), sc(&scanner),lookahead(Token()) {
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

ASTNode * Parser::transition(string functionName, functionPtr ptr) {
    admin->parserLog(functionName, PARSER_ENTER);
    
    ASTNode * node = (this->*ptr)();
    
    admin->parserLog(functionName, PARSER_EXIT);
	
	return node;
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
	admin->scannerLogEnd();
}

void Parser::startParsing(){
    lookahead = sc->getToken();
	admin->vec.push_back(lookahead);
    transition("program", &Parser::program);
}

// Finish me!!!!
ASTNode * Parser::program(){
	ASTNode * top;
    do{
        top = transition("declaration", &Parser::declaration);
    }while(lookahead.getTokenType() == sc->INT || lookahead.getTokenType() == sc->BOOL || lookahead.getTokenType() == sc->VOID);
	top->printNode(0);
}

ASTNode * Parser::declaration(){
	string idName = "";
	ASTNode * dNode = NULL;
	
    if(lookahead.getTokenType() == sc ->VOID){
        match(sc->VOID);
		idName = sc->getIdentifierName(lookahead.getAttributeValue());
        match(sc->ID);
		
        dNode = transition("funDecTail", &Parser::funDecTail);
		
		((ASTFunctionNode *)dNode)->declarationType = sc->VOID;
		((ASTFunctionNode *)dNode)->idName = idName;
    }
    else {
		int decType = 0;
		dNode = transition("nonVoidSpecifier", &Parser::nonVoidSpecifier);
		
		decType = ((ASTDeclarationNode *)dNode)->declarationType;
		idName = sc->getIdentifierName(lookahead.getAttributeValue());
        match(sc->ID);
		
		if(lookahead.getTokenType() == sc->LPAREN) {
			dNode = transition("funDecTail", &Parser::funDecTail);
			((ASTFunctionNode *)dNode)->declarationType = decType;
			((ASTFunctionNode *)dNode)->idName = idName;
		}
		else {
			dNode = transition("varDecTail", &Parser::varDecTail);
			((ASTVariableDeclarationNode *)dNode)->declarationType = decType;
			((ASTVariableDeclarationNode *)dNode)->idName = idName;
		}
    }
	
	return dNode;
}

ASTNode * Parser::nonVoidSpecifier(){
	ASTDeclarationNode * dNode = new ASTDeclarationNode;
    if(lookahead.getTokenType() == sc->INT) {
        match(sc->INT);
		dNode->declarationType = sc->INT;
	}
    else {
        match(sc->BOOL);
		dNode->declarationType = sc->BOOL;
	}
	
	return dNode;
}

ASTNode * Parser::varDecTail(){
	ASTVariableDeclarationNode * parent = new ASTVariableDeclarationNode, *vNode = parent;
	
    if(lookahead.getTokenType() == sc->LSQR){
        match(sc->LSQR);
        parent->arrayExp = ((ASTExpressionNode *)transition("addExp", &Parser::addExp));
        match(sc->RSQR);
		
		parent->isArray = true;
    }
    while(lookahead.getTokenType() == sc->COMMA){
        match(sc->COMMA);
        vNode->next = transition("varName", &Parser::varName);
		vNode = ((ASTVariableDeclarationNode *)vNode->next);
    }
    
    match(sc->SEMI);
	
	return parent;
}

ASTNode * Parser::varName(){
	string idName = "";
	ASTVariableDeclarationNode * vNode = new ASTVariableDeclarationNode;
	
	idName = sc->getIdentifierName(lookahead.getAttributeValue());
    match(sc->ID);
    
    if(lookahead.getTokenType() == sc->LSQR){
        match(sc->LSQR);
        vNode->arrayExp = ((ASTExpressionNode *)transition("addExp", &Parser::addExp));
        match(sc->RSQR);
		
		vNode->isArray = true;
    }
	
	vNode->idName = idName;
	
	return vNode;
}

ASTNode * Parser::funDecTail(){
	ASTFunctionNode * fNode = new ASTFunctionNode;
	ASTParamNode * pNode = NULL;
	ASTCompoundNode * cNode = NULL;
	
    match(sc->LPAREN);
    pNode = ((ASTParamNode *)transition("params", &Parser::params));
    match(sc->RPAREN);
    cNode = ((ASTCompoundNode *)transition("compoundStmt", &Parser::compoundStmt));
	
	fNode->param = pNode;
	fNode->compound = cNode;
	
	return fNode;
}

ASTNode * Parser::params(){
	ASTParamNode * parent = NULL, *pNode = NULL;
	
    if(lookahead.getTokenType() == sc->REF){
        parent = ((ASTParamNode *)transition("param", &Parser::param));
		pNode = parent;
        while(lookahead.getTokenType() == sc->COMMA){
            match(sc->COMMA);
            pNode->next = transition("param", &Parser::param);
			pNode = ((ASTParamNode *)pNode->next);
        }
    }
    else{
        match(sc->VOID);
    }
	
	return parent;
}

ASTNode * Parser::param(){
	ASTParamNode * pNode = new ASTParamNode;
	ASTDeclarationNode * dNode = NULL;
	string idName = "";
	
    if(lookahead.getTokenType() == sc->REF){
        match(sc->REF);
        dNode = ((ASTDeclarationNode *)transition("nonVoidSpecifier", &Parser::nonVoidSpecifier));
		idName = sc->getIdentifierName(lookahead.getAttributeValue());
        match(sc->ID);
		
		pNode->isRef = true;
		pNode->declarationType = dNode->declarationType;
		pNode->idName = idName;
    }
    else{
        dNode = ((ASTDeclarationNode *)transition("nonVoidSpecifier", &Parser::nonVoidSpecifier));
		idName = sc->getIdentifierName(lookahead.getAttributeValue());
        match(sc->ID);
		
        if(lookahead.getTokenType() == sc->LSQR){
            match(sc->LSQR);
            match(sc->RSQR);
			
			pNode->isArray = true;
        }
		
		pNode->declarationType = dNode->declarationType;
		pNode->idName = idName;
    }
	
	return pNode;
}

ASTNode * Parser::statement(){
    switch(lookahead.getTokenType()){
        case sc->ID:
			return transition("idStmt", &Parser::idStmt);
            break;
        case sc->LCRLY:
			return transition("compoundStmt", &Parser::compoundStmt);
            break;
        case sc->IF:
			return transition("ifStmt", &Parser::ifStmt);
            break;
        case sc->LOOP:
			return transition("loopStmt", &Parser::loopStmt);
            break;
        case sc->EXIT:
			return transition("exitStmt", &Parser::exitStmt);
            break;
        case sc->CONTINUE:
			return transition("continueStmt", &Parser::continueStmt);
            break;
        case sc->RETURN:
			return transition("returnStmt", &Parser::returnStmt);
            break;
        case sc->BRANCH:
			return transition("branchStmt", &Parser::branchStmt);
            break;
        default:
			return transition("nullStmt", &Parser::nullStmt);
            break;
    }
}

ASTNode * Parser::idStmt(){
	string idName = sc->getIdentifierName(lookahead.getAttributeValue());
	ASTStatementNode * sNode = NULL;
    match(sc->ID);
	
    if(lookahead.getTokenType() == sc->LSQR || lookahead.getTokenType() == sc->ASSIGN){
        sNode = ((ASTStatementNode *)transition("assignStmtTail", &Parser::assignStmtTail));
		((ASTAssignmentNode *)sNode)->idName = idName;
    }
    else{
        sNode = ((ASTStatementNode *)transition("callTail", &Parser::callTail));
		((ASTFunctionCallNode *)sNode)->idName = idName;
    }
	
	return sNode;
}

ASTNode * Parser::assignStmtTail(){
	ASTAssignmentNode * aNode = new ASTAssignmentNode;
	ASTExpressionNode * eNode = NULL;
	
    if(lookahead.getTokenType() == sc->LSQR){
        match(sc->LSQR);
        aNode->arrayExp = ((ASTExpressionNode *)transition("addExp", &Parser::addExp));
        match(sc->RSQR);
		
		aNode->isArray = true;
    }
    
    match(sc->ASSIGN);
    eNode = ((ASTExpressionNode *)transition("expression", &Parser::expression));
    match(sc->SEMI);
	
	aNode->exp = eNode;
	
	return aNode;
}

ASTNode * Parser::callTail(){
	ASTFunctionCallNode * fNode = new ASTFunctionCallNode;
	ASTExpressionNode * argument = NULL;
	
    match(sc->LPAREN);
        
    if(isExpressionLookahead()){
        argument = ((ASTExpressionNode *)transition("arguments", &Parser::arguments));
		fNode->argument = argument;
    }
    
    match(sc->RPAREN);
    match(sc->SEMI);
	
	return fNode;
}

ASTNode * Parser::arguments(){
	ASTExpressionNode * parent = NULL, *eNode = NULL;
    parent = ((ASTExpressionNode *)transition("expression", &Parser::expression));
	eNode = parent;
    while(lookahead.getTokenType() == sc->COMMA){
        match(sc->COMMA);
        eNode->next = transition("expression", &Parser::expression);
		eNode = ((ASTExpressionNode *)eNode->next);
    }
	
	return parent;
}

// May have to change "next" to a different construct (see diagram)
ASTNode * Parser::compoundStmt(){
	int decType = 0;
	string idName = "";
	ASTCompoundNode * cNode = new ASTCompoundNode;
	ASTDeclarationNode * dNode = new ASTDeclarationNode;
	ASTStatementNode * sNode = NULL;
	ASTNode * current = cNode;
	
    match(sc->LCRLY);
    while(lookahead.getTokenType() == sc->INT || lookahead.getTokenType() == sc->BOOL){
        dNode = ((ASTDeclarationNode *)transition("nonVoidSpecifier", &Parser::nonVoidSpecifier));
		idName = sc->getIdentifierName(lookahead.getAttributeValue());
        match(sc->ID);
		
		decType = dNode->declarationType;
		
        dNode = ((ASTDeclarationNode *)transition("varDecTail", &Parser::varDecTail));
		((ASTVariableDeclarationNode *)dNode)->declarationType = decType;
		((ASTVariableDeclarationNode *)dNode)->idName = idName;
		
		current->next = dNode;
		current = current->next;
		
		while(current->next != NULL) {
			((ASTVariableDeclarationNode *)current->next)->declarationType = dNode->declarationType;
			current = current->next;
		}
    }
    do{
        sNode = ((ASTStatementNode *)transition("statement", &Parser::statement));
		
		current->next = sNode;
		current = current->next;
    }while(isStatementLookahead());
     
    match(sc->RCRLY);
	
	return cNode;
}

ASTNode * Parser::ifStmt(){
	ASTIfNode * iNode = new ASTIfNode;
	
    match(sc->IF);
    match(sc->LPAREN);
    iNode->exp = ((ASTExpressionNode *)transition("expression", &Parser::expression));
    match(sc->RPAREN);
    iNode->statement = ((ASTStatementNode *)transition("statement", &Parser::statement));
    
    if(lookahead.getTokenType() == sc->ELSE){
        match(sc->ELSE);
        iNode->elseStatement = ((ASTStatementNode *)transition("statement", &Parser::statement));
    }
	
	return iNode;
}

ASTNode * Parser::loopStmt(){
	ASTLoopNode * lNode = new ASTLoopNode;
	ASTNode * current = lNode;
	
    match(sc->LOOP);
	lNode->statement = ((ASTStatementNode *)transition("statement", &Parser::statement));
	current = lNode->statement;
    while(isStatementLookahead()){
        current->next = transition("statement", &Parser::statement);
		current = current->next;
	}
    match(sc->END);
    match(sc->SEMI);
	
	return lNode;
}

ASTNode * Parser::exitStmt(){
	ASTMarkerNode * marker = new ASTMarkerNode;
	
	marker->type = sc->EXIT;
    match(sc->EXIT);
    match(sc->SEMI);
	
	return marker;
}

// NEED MORE STATEMENTS

ASTNode * Parser::continueStmt(){
    match(sc->CONTINUE);
    match(sc->SEMI);
}

ASTNode * Parser::returnStmt(){
    match(sc->RETURN);
    if(isExpressionLookahead()){
        transition("expression", &Parser::expression);
    }
    match(sc->SEMI);
}

ASTNode * Parser::nullStmt(){
    match(sc->SEMI);
}

ASTNode * Parser::branchStmt(){
    match(sc->BRANCH);
    match(sc->LPAREN);
    transition("addExp", &Parser::addExp);
    match(sc->RPAREN);
    
    do{
        transition("caseStmt", &Parser::caseStmt);
    }while(lookahead.getTokenType() == sc->CASE || lookahead.getTokenType() == sc->DEFAULT);
    
    match(sc->END);
    match(sc->SEMI);
}

//NOTE colon doesn't exist in language???
ASTNode * Parser::caseStmt(){
    if(lookahead.getTokenType() == sc->CASE){
        match(sc->CASE);
        match(sc->NUM);
       // match(sc->COLON);
        transition("statement", &Parser::statement);
    }
    else{
        match(sc->DEFAULT);
        //match(sc->COLON);
        transition("statement", &Parser::statement);
    }
}

// END REMAINING STATEMENTS

ASTNode * Parser::expression(){
    ASTExpressionNode * exp = ((ASTExpressionNode *)transition("addExp", &Parser::addExp));
    if(isRelopLookahead())
    {
		ASTBinaryNode * next = new ASTBinaryNode;
        next->oper = lookahead.getTokenType();
		match(lookahead.getTokenType());
        next->right = ((ASTExpressionNode *)transition("addExp", &Parser::addExp));
		next->left = exp;
		//next->type = ((ASTExpressionNode *)right)->type;
		exp = next;
    }
	
	return exp;
}

ASTNode * Parser::addExp(){
	bool isNeg = false;
	ASTExpressionNode * exp = NULL;
	
    if(lookahead.getTokenType() == sc->MINUS){
        match(sc->MINUS);
		isNeg = true;
    }
    exp = ((ASTExpressionNode *)transition("term", &Parser::term));
	if(isNeg) {
		ASTUnaryNode * next = new ASTUnaryNode;
		next->operation = sc->MINUS;
		next->operand = exp;
		//next->type = ((ASTExpressionNode *)exp)->type;
		exp = next;
	}
    while(isAddopLookahead()){
		ASTBinaryNode * next = new ASTBinaryNode;
		next->oper = lookahead.getTokenType();
		match(lookahead.getTokenType());
		next->right = ((ASTExpressionNode *)transition("term", &Parser::term));
		next->left = exp;
		//next->type = ((ASTExpressionNode *)right)->type;
		exp = next;
    }
	
	return exp;
}

ASTNode * Parser::term(){
    ASTExpressionNode * exp = ((ASTExpressionNode *)transition("factor", &Parser::factor));
    while(isMultopLookahead()){
        ASTBinaryNode * next = new ASTBinaryNode;
        next->oper = lookahead.getTokenType();
		match(lookahead.getTokenType());
        next->right = ((ASTExpressionNode *)transition("factor", &Parser::factor));
		next->left = exp;
		//next->type = ((ASTExpressionNode *)right)->type;
		exp = next;
    }
	
	return exp;
}

ASTNode * Parser::factor(){
    if(lookahead.getTokenType() == sc->ID){
        return transition("idFactor", &Parser::idFactor);
    }
    else{
        return transition("nidFactor", &Parser::nidFactor);
    }
}

// Might be doing parenthesized expressions wrong
ASTNode * Parser::nidFactor(){
	ASTExpressionNode * eNode = NULL;
    switch(lookahead.getTokenType()){
        case sc->NOT:
			eNode = new ASTUnaryNode;
            match(sc->NOT);
			((ASTUnaryNode *)eNode)->operand = ((ASTExpressionNode *)transition("factor", &Parser::factor));
			((ASTUnaryNode *)eNode)->operation = sc->NOT;
			((ASTUnaryNode *)eNode)->type = ((ASTUnaryNode *)eNode)->operand->type;
            break;
        case sc->LPAREN:
            match(sc->LPAREN);
			eNode = ((ASTExpressionNode *)transition("expression", &Parser::expression));
            match(sc->RPAREN);
            break;
        case sc->NUM:
			eNode = new ASTLiteralNode;
			eNode->type = sc->INT;
			((ASTLiteralNode *)eNode)->value = lookahead.getAttributeValue();
            match(sc->NUM);
            break;
        default:
			eNode = new ASTLiteralNode;
			eNode->type = sc->BLIT;
			((ASTLiteralNode *)eNode)->value = lookahead.getAttributeValue();
            match(sc->BLIT);
            break;        
    }
	
	return eNode;
}

ASTNode * Parser::idFactor(){
	string idName = sc->getIdentifierName(lookahead.getAttributeValue());
	ASTVariableNode * varNode = new ASTVariableNode;
	
    match(sc->ID);
	varNode->idName = idName;
    if(lookahead.getTokenType() == sc->LPAREN){
        varNode->func = ((ASTFunctionCallNode *)transition("callTail", &Parser::callTail));
    }
    else if(lookahead.getTokenType() == sc->LSQR){
		varNode->isArray = true;
		
		match(sc->LSQR);
		varNode->arrayExp = ((ASTExpressionNode *)transition("addExp", &Parser::addExp));
		match(sc->RSQR);
    }
	
	return varNode;
}

void Parser:: match(int expected){
    
    if(lookahead.getTokenType() == expected){
        lookahead = sc->getToken();
		admin->vec.push_back(lookahead);
    }
    else{
        //syntaxError();
    }
}

bool Parser:: isStatementLookahead(){
   
    switch(lookahead.getTokenType()){
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
    switch(lookahead.getTokenType()){
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
    switch(lookahead.getTokenType()){
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
    switch(lookahead.getTokenType()){
        case sc->PLUS:
        case sc->MINUS:
        case sc->OR:
        case sc->ORELSE:return true;
    }
    return false;
}

bool Parser:: isMultopLookahead(){
    switch(lookahead.getTokenType()){
        case sc->MULT:
        case sc->DIV:
        case sc->MOD:
        case sc->AND:
        case sc->ANDTHEN:return true;
    }
    return false;
}