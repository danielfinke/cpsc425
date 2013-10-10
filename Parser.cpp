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
#include "ASTBranchNode.h"
#include "ASTCaseNode.h"
#include "ASTReturnNode.h"
#include <iostream>

Parser::Parser(void) : admin(NULL), sc(NULL), lookahead(Token()), astTop(NULL)
{
}
Parser::Parser(Admin& adminMod, Scanner& scanner) : admin(&adminMod), sc(&scanner),lookahead(Token()),
		astTop(NULL)
{
}
/* We do not create new instances of Admin or Scanner in either of the copy constructor/assignment operators
 * because there should only be one instance available (these functions should rarely be used)
 */
Parser::Parser(const Parser &other) : admin(other.admin), sc(other.sc),lookahead(other.lookahead),
		astTop(other.astTop)
{
}
Parser& Parser::operator= (const Parser &rhs)
{
    // do the copy
    admin = rhs.admin;
	sc = rhs.sc;
    lookahead = rhs.lookahead;
	astTop = rhs.astTop;
 
    // return the existing object
    return *this;
}

Parser::~Parser(void)
{
	// The Parser does not delete the Scanner or Admin instances, because it does not have ownership.
	delete astTop;
}

/*Transition function is used to avoid trace messages within the code
 * it Calls the function in the parameters, as well as passes information
 * to the admin and calls the log function of admin.
 */
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

/*Starts the parsing of the code, initates the lookahead to the first token
 * from the scanner, sets the astTop to the top node of the program through 
 * the mutually recursive calls started from the program() call (done through
 * transition method). 
 * 
 * Calls to the admin are made to do logging
 */
void Parser::startParsing(){
    lookahead = sc->getToken();
	admin->vec.push_back(lookahead);
	admin->scannerLogEnd();
    astTop = transition("program", &Parser::program);
	admin->parserLog(astTop);
}

/*The following methods are based on the parsing grammar given by Jernej Polajner
 * and are a set of mutually recursive methods.
 * Each production is represented a method which matches terminal symbols and 
 * calls the methods of the non-terminal symbols. There a few exceptions where
 * production have been combined into one method in order to create ASTNodes
 * with the required information. ASTNodes are created and chained according to
 * the construct being built.These are documented below and within the 
 * Parser Basic document.
 */

//program -> {|declaration|}+
ASTNode * Parser::program(){
	ASTDeclarationNode * parent = ((ASTDeclarationNode *)transition("declaration", &Parser::declaration));
	ASTDeclarationNode * current = parent;
	while(current->next != NULL) {
		current = ((ASTDeclarationNode *)current->next);
	}
	while(lookahead.getTokenType() == sc->INT || lookahead.getTokenType() == sc->BOOL || lookahead.getTokenType() == sc->VOID) {
		current->next = ((ASTDeclarationNode *)transition("declaration", &Parser::declaration));
		while(current->next != NULL) {
			current = ((ASTDeclarationNode *)current->next);
		}
	}
	
	return parent;
}


//method for declaration production rule (including dec-tail production rule)
/*includes the declaration production rule as well as the
 * dec-tail production rule.This is so we can properly cast the declaration as 
 * either a functionNode or a variableDeclarationNode, and still get all relevant
 * information 
 */
ASTNode * Parser::declaration(){
	int id =0;
	ASTNode * dNode = NULL;
	
    if(lookahead.getTokenType() == sc ->VOID){
        match(sc->VOID);
		id = lookahead.getAttributeValue();
        match(sc->ID);
		
        dNode = transition("funDecTail", &Parser::funDecTail);
		
		((ASTFunctionNode *)dNode)->declarationType = sc->VOID;
		((ASTFunctionNode *)dNode)->id = id;
    }
    else {
		int decType = 0;
		dNode = transition("nonVoidSpecifier", &Parser::nonVoidSpecifier);
		
		decType = ((ASTDeclarationNode *)dNode)->declarationType;
		id = lookahead.getAttributeValue();
        match(sc->ID);
		//Start dec-tail production rule
		if(lookahead.getTokenType() == sc->LPAREN) {
			dNode = transition("funDecTail", &Parser::funDecTail);
			((ASTFunctionNode *)dNode)->declarationType = decType;
			((ASTFunctionNode *)dNode)->id = id;
		}
		else {
			dNode = transition("varDecTail", &Parser::varDecTail);
			((ASTVariableDeclarationNode *)dNode)->declarationType = decType;
			((ASTVariableDeclarationNode *)dNode)->id = id;
		}
                //End dec-tail production rule
    }
	
	return dNode;
}
//method for nonvoid-specifier production rule
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

//method for var-dec-tail production rule
ASTNode * Parser::varDecTail(){
	ASTVariableDeclarationNode * parent = new ASTVariableDeclarationNode, *vNode = parent;
	
    if(lookahead.getTokenType() == sc->LSQR){
        match(sc->LSQR);
        //parent->arrayExp = ((ASTExpressionNode *)transition("addExp", &Parser::addExp));
		parent->arrayExp = dynamic_cast<ASTExpressionNode *>(transition("addExp", &Parser::addExp));
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

//method for var-name production rule
ASTNode * Parser::varName(){
	int id =0;
	ASTVariableDeclarationNode * vNode = new ASTVariableDeclarationNode;
	
	id = lookahead.getAttributeValue();
    match(sc->ID);
    
    if(lookahead.getTokenType() == sc->LSQR){
        match(sc->LSQR);
        //vNode->arrayExp = ((ASTExpressionNode *)transition("addExp", &Parser::addExp));
		vNode->arrayExp = dynamic_cast<ASTExpressionNode *>(transition("addExp", &Parser::addExp));
        match(sc->RSQR);
		
		vNode->isArray = true;
    }
	
	vNode->id = id;
	
	return vNode;
}

//method for fun-dec-tail production rule
ASTNode * Parser::funDecTail(){
	ASTFunctionNode * fNode = new ASTFunctionNode;
	ASTParamNode * pNode = NULL;
	ASTCompoundNode * cNode = NULL;
	
    match(sc->LPAREN);
    pNode = ((ASTParamNode *)transition("params", &Parser::params));
    match(sc->RPAREN);
    //cNode = ((ASTCompoundNode *)transition("compoundStmt", &Parser::compoundStmt));
	cNode = dynamic_cast<ASTCompoundNode *>(transition("compoundStmt", &Parser::compoundStmt));
	
	fNode->param = pNode;
	fNode->compound = cNode;
	
	return fNode;
}

//method for params production rule
ASTNode * Parser::params(){
	ASTParamNode * parent = NULL, *pNode = NULL;
	
    if(lookahead.getTokenType() != sc->VOID){
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

//method for param procution rule
ASTNode * Parser::param(){
	ASTParamNode * pNode = new ASTParamNode;
	ASTDeclarationNode * dNode = NULL;
	int id =0;
	
    if(lookahead.getTokenType() == sc->REF){
        match(sc->REF);
        dNode = ((ASTDeclarationNode *)transition("nonVoidSpecifier", &Parser::nonVoidSpecifier));
		id = lookahead.getAttributeValue();
        match(sc->ID);
		
		pNode->isRef = true;
		pNode->declarationType = dNode->declarationType;
		pNode->id = id;
    }
    else{
        dNode = ((ASTDeclarationNode *)transition("nonVoidSpecifier", &Parser::nonVoidSpecifier));
		id = lookahead.getAttributeValue();
        match(sc->ID);
		
        if(lookahead.getTokenType() == sc->LSQR){
            match(sc->LSQR);
            match(sc->RSQR);
			
			pNode->isArray = true;
        }
		
		pNode->declarationType = dNode->declarationType;
		pNode->id = id;
    }
	
	return pNode;
}

//method for statement production rule
ASTNode * Parser::statement(){
    switch(lookahead.getTokenType()){
        case Scanner::ID:
			return transition("idStmt", &Parser::idStmt);
            break;
        case Scanner::LCRLY:
			return transition("compoundStmt", &Parser::compoundStmt);
            break;
        case Scanner::IF:
			return transition("ifStmt", &Parser::ifStmt);
            break;
        case Scanner::LOOP:
			return transition("loopStmt", &Parser::loopStmt);
            break;
        case Scanner::EXIT:
			return transition("exitStmt", &Parser::exitStmt);
            break;
        case Scanner::CONTINUE:
			return transition("continueStmt", &Parser::continueStmt);
            break;
        case Scanner::RETURN:
			return transition("returnStmt", &Parser::returnStmt);
            break;
        case Scanner::BRANCH:
			return transition("branchStmt", &Parser::branchStmt);
            break;
        default:
			return transition("nullStmt", &Parser::nullStmt);
            break;
    }
}


//method for id-stmt production rule (including call-tail production rule)
/*idSmt contains the id-stmt produciton rule as well as call-tail production rule. 
 * This is so we could properly cast the Astnode to either ASTAssignmentnode or
 * ASTFunctionCall node, while providing the relevant information for the node
 */
ASTNode * Parser::idStmt(){
	int id = lookahead.getAttributeValue();
	ASTStatementNode * sNode = NULL;
    match(sc->ID);
	
    if(lookahead.getTokenType() == sc->LSQR || lookahead.getTokenType() == sc->ASSIGN){
        //sNode = ((ASTStatementNode *)transition("assignStmtTail", &Parser::assignStmtTail));
		sNode = dynamic_cast<ASTStatementNode *>(transition("assignStmtTail", &Parser::assignStmtTail));
		((ASTAssignmentNode *)sNode)->id = id;
    }
    else{
        //Start call-tail production rule
        //sNode = ((ASTStatementNode *)transition("callTail", &Parser::callTail));
		sNode = dynamic_cast<ASTStatementNode *>(transition("callTail", &Parser::callTail));
		((ASTFunctionCallNode *)sNode)->id = id;
		match(sc->SEMI);
       //end call-tail production rule
    }
	
	return sNode;
}

//method for assign-stmt-tail production rule
ASTNode * Parser::assignStmtTail(){
	ASTAssignmentNode * aNode = new ASTAssignmentNode;
	ASTExpressionNode * eNode = NULL;
	
    if(lookahead.getTokenType() == sc->LSQR){
        match(sc->LSQR);
        //aNode->arrayExp = ((ASTExpressionNode *)transition("addExp", &Parser::addExp));
		aNode->arrayExp = dynamic_cast<ASTExpressionNode *>(transition("addExp", &Parser::addExp));
        match(sc->RSQR);
		
		aNode->isArray = true;
    }
    
    match(sc->ASSIGN);
    //eNode = ((ASTExpressionNode *)transition("expression", &Parser::expression));
	eNode = dynamic_cast<ASTExpressionNode *>(transition("expression", &Parser::expression));
    match(sc->SEMI);
	
	aNode->exp = eNode;
	
	return aNode;
}

//method for call-tail production rule
ASTNode * Parser::callTail(){
	ASTFunctionCallNode * fNode = new ASTFunctionCallNode;
	ASTExpressionNode * argument = NULL;
	
    match(sc->LPAREN);
        
    if(isExpressionLookahead()){
        //argument = ((ASTExpressionNode *)transition("arguments", &Parser::arguments));
		argument = dynamic_cast<ASTExpressionNode *>(transition("arguments", &Parser::arguments));
		fNode->argument = argument;
    }
    
    match(sc->RPAREN);
	
	return ((ASTExpressionNode *)fNode);
}

//method for arguments production rule
ASTNode * Parser::arguments(){
	ASTExpressionNode * parent = NULL, *eNode = NULL;
    //parent = ((ASTExpressionNode *)transition("expression", &Parser::expression));
	parent = dynamic_cast<ASTExpressionNode *>(transition("expression", &Parser::expression));
	eNode = parent;
    while(lookahead.getTokenType() == sc->COMMA){
        match(sc->COMMA);
        eNode->next = transition("expression", &Parser::expression);
		//eNode = ((ASTExpressionNode *)eNode->next);
		eNode = dynamic_cast<ASTExpressionNode *>(eNode->next);
    }
	
	return parent;
}

//method for compound-stm production rule
ASTNode * Parser::compoundStmt(){
	int decType = 0;
	int id =0;
	ASTCompoundNode * cNode = new ASTCompoundNode;
	ASTDeclarationNode * dNode = new ASTDeclarationNode;
	ASTStatementNode * sNode = NULL;
	ASTNode * current = cNode;
	
    match(sc->LCRLY);
    while(lookahead.getTokenType() == sc->INT || lookahead.getTokenType() == sc->BOOL){
        dNode = ((ASTDeclarationNode *)transition("nonVoidSpecifier", &Parser::nonVoidSpecifier));
		id = lookahead.getAttributeValue();
        match(sc->ID);
		
		decType = dNode->declarationType;
		
        dNode = ((ASTDeclarationNode *)transition("varDecTail", &Parser::varDecTail));
		((ASTVariableDeclarationNode *)dNode)->declarationType = decType;
		((ASTVariableDeclarationNode *)dNode)->id = id;
		
		// Make sure that we start by adding to compound's declarations, then chaining later
		if(cNode->dec == NULL) {
			cNode->dec = dNode;
			current = cNode->dec;
		}
		else {
			current->next = dNode;
			current = current->next;
		}
		
		while(current->next != NULL) {
			((ASTVariableDeclarationNode *)current->next)->declarationType = dNode->declarationType;
			current = current->next;
		}
    }
    do{
        //sNode = ((ASTStatementNode *)transition("statement", &Parser::statement));
		sNode = dynamic_cast<ASTStatementNode *>(transition("statement", &Parser::statement));
		
		// Make sure that we start by adding to compound's statements, then chaining later
		if(cNode->statement == NULL) {
			cNode->statement = sNode;
			current = cNode->statement;
		}
		else {
			current->next = sNode;
			current = current->next;
		}
    }while(isStatementLookahead());
     
    match(sc->RCRLY);
	
	return cNode;
}

//method for if-stmt production rule
ASTNode * Parser::ifStmt(){
	ASTIfNode * iNode = new ASTIfNode;
	
    match(sc->IF);
    match(sc->LPAREN);
    //iNode->exp = ((ASTExpressionNode *)transition("expression", &Parser::expression));
	iNode->exp = dynamic_cast<ASTExpressionNode *>(transition("expression", &Parser::expression));
    match(sc->RPAREN);
    //iNode->statement = ((ASTStatementNode *)transition("statement", &Parser::statement));
	iNode->statement = dynamic_cast<ASTStatementNode *>(transition("statement", &Parser::statement));
    
    if(lookahead.getTokenType() == sc->ELSE){
        match(sc->ELSE);
        //iNode->elseStatement = ((ASTStatementNode *)transition("statement", &Parser::statement));
		iNode->elseStatement = dynamic_cast<ASTStatementNode *>(transition("statement", &Parser::statement));
    }
	
	return iNode;
}

//method for loop-stmt production rule
ASTNode * Parser::loopStmt(){
	ASTLoopNode * lNode = new ASTLoopNode;
	ASTNode * current = lNode;
	
    match(sc->LOOP);
	//lNode->statement = ((ASTStatementNode *)transition("statement", &Parser::statement));
	lNode->statement = dynamic_cast<ASTStatementNode *>(transition("statement", &Parser::statement));
	current = lNode->statement;
    while(isStatementLookahead()){
        current->next = transition("statement", &Parser::statement);
		current = current->next;
	}
    match(sc->END);
    match(sc->SEMI);
	
	return lNode;
}

//method for exit-stmt production rule
ASTNode * Parser::exitStmt(){
	ASTMarkerNode * marker = new ASTMarkerNode;
	
	marker->type = sc->EXIT;
    match(sc->EXIT);
    match(sc->SEMI);
	
	return marker;
}

//method for continue-stmt production rule
ASTNode * Parser::continueStmt(){
    ASTMarkerNode * marker = new ASTMarkerNode;
    
    marker ->type = sc->CONTINUE;
    
    match(sc->CONTINUE);
    match(sc->SEMI);
    
    return marker;
}

//method for return-stmt production rule
ASTNode * Parser::returnStmt(){
    ASTReturnNode * rNode = new ASTReturnNode;

    match(sc->RETURN);
    if(isExpressionLookahead()){
		//rNode->expression = ((ASTExpressionNode *)transition("expression", &Parser::expression));
		rNode->expression = dynamic_cast<ASTExpressionNode *>(transition("expression", &Parser::expression));
    }
    
    match(sc->SEMI);
    return rNode;
}

//method for null-stmt production rule
ASTNode * Parser::nullStmt(){   
    ASTMarkerNode * marker = new ASTMarkerNode;
    marker ->type = sc->NULLSTMT;
    match(sc->SEMI);
    
    return marker;
}

//method for branch-stmt production rule
ASTNode * Parser::branchStmt(){
    ASTBranchNode * bNode = new ASTBranchNode;
    ASTCaseNode *current = NULL;
    match(sc->BRANCH);
    match(sc->LPAREN);
    //bNode ->expression =((ASTExpressionNode *)transition("addExp", &Parser::addExp));
	bNode->expression = dynamic_cast<ASTExpressionNode *>(transition("addExp", &Parser::addExp));
    match(sc->RPAREN);
    
    
    //bNode->firstCase= ((ASTCaseNode *)transition("caseStmt", &Parser::caseStmt));
	bNode->firstCase = dynamic_cast<ASTCaseNode *>(transition("caseStmt", &Parser::caseStmt));
    current =bNode->firstCase;
    while(lookahead.getTokenType() == sc->CASE || lookahead.getTokenType() == sc->DEFAULT){
        //current->nextCase = ((ASTCaseNode *)transition("caseStmt", &Parser::caseStmt));
		current->nextCase = dynamic_cast<ASTCaseNode *>(transition("caseStmt", &Parser::caseStmt));
        current = current->nextCase;
    }
    
    match(sc->END);
    match(sc->SEMI);
    
    return bNode;
}

//method for case-stmt production rule
ASTNode * Parser::caseStmt(){
    ASTCaseNode * cNode = new ASTCaseNode;
    if(lookahead.getTokenType() == sc->CASE){
        cNode->type = sc->CASE;
        match(sc->CASE);
        cNode->num = lookahead.getAttributeValue();
        match(sc->NUM);
        match(sc->COLON);
        //cNode->statement = ((ASTStatementNode *)transition("statement", &Parser::statement));
		cNode->statement = dynamic_cast<ASTStatementNode *>(transition("statement", &Parser::statement));
    }
    else{
        cNode->type = sc->DEFAULT;
        match(sc->DEFAULT);
        match(sc->COLON);
        //cNode->statement = ((ASTStatementNode *)transition("statement", &Parser::statement));
		cNode->statement = dynamic_cast<ASTStatementNode *>(transition("statement", &Parser::statement));
    }
    
    return cNode;
}

//method for expression production rule
// Commented out expression types because they will be taken care of later during semantic analysis
ASTNode * Parser::expression(){
    //ASTExpressionNode * exp = ((ASTExpressionNode *)transition("addExp", &Parser::addExp));
    ASTExpressionNode * exp = dynamic_cast<ASTExpressionNode *>(transition("addExp", &Parser::addExp));
    if(isRelopLookahead())
    {
		ASTBinaryNode * next = new ASTBinaryNode;
        next->oper = lookahead.getTokenType();
		match(lookahead.getTokenType());
        //next->right = ((ASTExpressionNode *)transition("addExp", &Parser::addExp));
		next->right = dynamic_cast<ASTExpressionNode *>(transition("addExp", &Parser::addExp));
		next->left = exp;
		//next->type = ((ASTExpressionNode *)right)->type;
		exp = next;
    }
	
	return exp;
}

//method for add-exp production rule (containing uminus and addop production rules )
/*
 */
ASTNode * Parser::addExp(){
	bool isNeg = false;
	ASTExpressionNode * exp = NULL;
	
    if(lookahead.getTokenType() == sc->MINUS){
        //start uMinus production rule
        match(sc->MINUS);
        //end uminus production rule
		isNeg = true;
    }
    //exp = ((ASTExpressionNode *)transition("term", &Parser::term));
	exp = dynamic_cast<ASTExpressionNode *>(transition("term", &Parser::term));
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
		//next->right = ((ASTExpressionNode *)transition("term", &Parser::term));
		next->right = dynamic_cast<ASTExpressionNode *>(transition("term", &Parser::term));
		next->left = exp;
		//next->type = ((ASTExpressionNode *)right)->type;
		exp = next;
    }
	
	return exp;
}

//method for term production rule
ASTNode * Parser::term(){
    //ASTExpressionNode * exp = ((ASTExpressionNode *)transition("factor", &Parser::factor));
    ASTExpressionNode * exp = dynamic_cast<ASTExpressionNode *>(transition("factor", &Parser::factor));
    while(isMultopLookahead()){
        ASTBinaryNode * next = new ASTBinaryNode;
        next->oper = lookahead.getTokenType();
		match(lookahead.getTokenType());
        //next->right = ((ASTExpressionNode *)transition("factor", &Parser::factor));
		next->right = dynamic_cast<ASTExpressionNode *>(transition("factor", &Parser::factor));
		next->left = exp;
		//next->type = ((ASTExpressionNode *)right)->type;
		exp = next;
    }
	
	return exp;
}

//method for factor production rule
ASTNode * Parser::factor(){
    if(lookahead.getTokenType() == sc->ID){
        return transition("idFactor", &Parser::idFactor);
    }
    else{
        return transition("nidFactor", &Parser::nidFactor);
    }
}

//method for nid-factor production rule
ASTNode * Parser::nidFactor(){
	ASTExpressionNode * eNode = NULL;
    switch(lookahead.getTokenType()){
        case Scanner::NOT:
			eNode = new ASTUnaryNode;
            match(sc->NOT);
			//((ASTUnaryNode *)eNode)->operand = ((ASTExpressionNode *)transition("factor", &Parser::factor));
			((ASTUnaryNode *)eNode)->operand = dynamic_cast<ASTExpressionNode *>(transition("factor", &Parser::factor));
			((ASTUnaryNode *)eNode)->operation = sc->NOT;
			((ASTUnaryNode *)eNode)->type = ((ASTUnaryNode *)eNode)->operand->type;
            break;
        case Scanner::LPAREN:
            match(sc->LPAREN);
			//eNode = ((ASTExpressionNode *)transition("expression", &Parser::expression));
			eNode = dynamic_cast<ASTExpressionNode *>(transition("expression", &Parser::expression));
            match(sc->RPAREN);
            break;
        case Scanner::NUM:
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

//method for id-factor production rule (containing id-tail production rule (containing var-tail production rule))
/*contains id-factor production rule and id-tail production rule so that we can
 * correctly cast the ASTNode to either ASTFunctioncallNode or ASTvariableNode
 * while getting all relevant information for the node.
 * The var-tail production rule was encompassed so that we could attain the
 * isArray information for the Node
 */
ASTNode * Parser::idFactor(){
	int id = lookahead.getAttributeValue();
	ASTExpressionNode * exp = NULL;
	
    match(sc->ID);
    
    //Start id-tail production rule
    if(lookahead.getTokenType() == sc->LPAREN){
        //exp = ((ASTExpressionNode *)transition("callTail", &Parser::callTail));
		exp = dynamic_cast<ASTExpressionNode *>(transition("callTail", &Parser::callTail));
		((ASTFunctionCallNode *)exp)->id = id;
    }
    else {
		exp = new ASTVariableNode;
		((ASTVariableNode *)exp)->id = id;
                
                //Start var-tail production rule
		if(lookahead.getTokenType() == sc->LSQR){
			match(sc->LSQR);
			//((ASTVariableNode *)exp)->arrayExp = ((ASTExpressionNode *)transition("addExp", &Parser::addExp));
			((ASTVariableNode *)exp)->arrayExp = dynamic_cast<ASTExpressionNode *>(transition("addExp", &Parser::addExp));
			match(sc->RSQR);

			((ASTVariableNode *)exp)->isArray = true;
		}
                //end var-tail production rule
	}
    //end id-tail production rule
	
	return exp;
}

/*matches terminal symbols by checking if the lookahead is equal to the expected
 * terminal symbol. If so, it gets the next token as the new lookahead and sends
 * match and load information to the admin for traceing. if not in
 * sends error information to the admin for logging.
 */

void Parser:: match(int expected){
    if(lookahead.getTokenType() == expected){
		admin->parserLog(expected, PARSER_MATCH);
        lookahead = sc->getToken();
		admin->vec.push_back(lookahead);
		admin->scannerLogEnd();
		admin->parserLog(lookahead.getTokenType(), PARSER_LOAD);
    }
    else{
       admin->syntaxError(expected, lookahead.getTokenType());
    }
}

//returns true if lookahead is part of statement first set
bool Parser:: isStatementLookahead(){
   
    switch(lookahead.getTokenType()){
        //case Scanner::LCRLY:
	case Scanner::LCRLY:
        case Scanner::IF:
        case Scanner::LOOP:
        case Scanner::EXIT:
        case Scanner::CONTINUE:
        case Scanner::RETURN:
        case Scanner::SEMI:
        case Scanner::ID:
        case Scanner::BRANCH:return true;   
    }
    return false;
}

//returns true if lookahead is part of arguments/expression/addExp first set
//same first set for Arguments; Expression; addExp;
bool Parser:: isExpressionLookahead(){
    switch(lookahead.getTokenType()){
        case Scanner::MINUS:
        case Scanner::NOT:
        case Scanner::LPAREN:
        case Scanner::NUM:
        case Scanner::BLIT:
        case Scanner::ID:return true;          
    }
    return false;
}

//returns true if lookahead is part of relop first set
bool Parser:: isRelopLookahead(){
    switch(lookahead.getTokenType()){
        case Scanner::LTEQ:
        case Scanner::LT:
        case Scanner::GT:
        case Scanner::GTEQ:
        case Scanner::EQ:
        case Scanner::NEQ:return true;
    }
    return false;
}

//returns true if lookahead is part of addop first set
bool Parser:: isAddopLookahead(){
    switch(lookahead.getTokenType()){
        case Scanner::PLUS:
        case Scanner::MINUS:
        case Scanner::OR:
        case Scanner::ORELSE:return true;
    }
    return false;
}

//returns true if lookahead is part of multop first set
bool Parser:: isMultopLookahead(){
    switch(lookahead.getTokenType()){
        case Scanner::MULT:
        case Scanner::DIV:
        case Scanner::MOD:
        case Scanner::AND:
        case Scanner::ANDTHEN:return true;
    }
    return false;
}
