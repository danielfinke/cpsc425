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
#include "SyncSetBuilder.h"
#include <iostream>
#include <algorithm>

Parser::Parser(void) : errorCount(0), loopNesting(0),
		admin(NULL), sc(NULL), lookahead(Token()), astTop(NULL)
{
}
Parser::Parser(Admin& adminMod, Scanner& scanner) : errorCount(0),
		loopNesting(0),
		admin(&adminMod), sc(&scanner),lookahead(Token()),
		astTop(NULL)
{
}
/* We do not create new instances of Admin or Scanner in either of the copy constructor/assignment operators
 * because there should only be one instance available (these functions should rarely be used)
 */
Parser::Parser(const Parser &other) : errorCount(other.errorCount),
		loopNesting(other.loopNesting),
		admin(other.admin), sc(other.sc),lookahead(other.lookahead),
		astTop(other.astTop)
{
}
Parser& Parser::operator= (const Parser &rhs)
{
    // do the copy
	errorCount = rhs.errorCount;
	loopNesting = rhs.loopNesting;
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
ASTNode * Parser::transition(string functionName, functionPtr ptr, vector<int> syncSet) {
	vector<int> supplementary = SyncSetBuilder::getSyncSet(functionName);
	
    admin->parserLog(functionName, PARSER_ENTER);
    
	syncSet.insert(syncSet.end(), supplementary.begin(), supplementary.end());
    ASTNode * node = (this->*ptr)(syncSet);
    
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
ASTNode* Parser::startParsing(){
	vector<int> emptySyncSet;
	
	SyncSetBuilder::buildSyncSetMap();
	
	lookahead = sc->getToken();
	admin->vec.push_back(lookahead);
	admin->scannerLogEnd();
	
    astTop = transition("program", &Parser::program, emptySyncSet);
	
	if(errorCount == 0) {
		admin->parserLog(astTop);
                return astTop;
	}
	else if(admin->getOutputAST()) {
		admin->cancelAST();
                return NULL;
	}
}

/*The following methods are based on the parsing grammar given by Jernej Polajnar
 * and are a set of mutually recursive methods.
 * Each production is represented a method which matches terminal symbols and 
 * calls the methods of the non-terminal symbols. There a few exceptions where
 * production have been combined into one method in order to create ASTNodes
 * with the required information. ASTNodes are created and chained according to
 * the construct being built.These are documented below and within the 
 * Parser Basic document.
 */

//program -> {|declaration|}+
ASTNode * Parser::program(vector<int> syncSet){
	ASTDeclarationNode * parent;
	ASTDeclarationNode * current;
	
	parent = ((ASTDeclarationNode *)transition("declaration", &Parser::declaration, syncSet));
	current = parent;
    current->isGlobalDec = true;
	
	while(current->next != NULL) {
		current = ((ASTDeclarationNode *)current->next);
        current->isGlobalDec = true;
	}
	//while(lookahead.getTokenType() == sc->INT || lookahead.getTokenType() == sc->BOOL || lookahead.getTokenType() == sc->VOID) {
	while(lookahead.getTokenType() != sc->ENDFILE) {
		current->next = ((ASTDeclarationNode *)transition("declaration", &Parser::declaration, syncSet));
		current->isGlobalDec = true;
        while(current->next != NULL) {
			current = ((ASTDeclarationNode *)current->next);
            current->isGlobalDec = true;
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
ASTNode * Parser::declaration(vector<int> syncSet){
	int id = 0;
	ASTNode * dNode = new ASTFunctionNode;
	
    if(lookahead.getTokenType() == sc->VOID) {
		
        if(match(sc->VOID, syncSet)) {

			id = lookahead.getAttributeValue();

			if(match(sc->ID, syncSet)) {

				dNode = transition("funDecTail", &Parser::funDecTail, syncSet);

				((ASTFunctionNode *)dNode)->declarationType = sc->VOID;
				((ASTFunctionNode *)dNode)->id = id;
			}
		}
    }
    else if(lookahead.getTokenType() == sc->INT || lookahead.getTokenType() == sc->BOOL) {
		int decType = 0;
		dNode = transition("nonVoidSpecifier", &Parser::nonVoidSpecifier, syncSet);
		
		decType = ((ASTDeclarationNode *)dNode)->declarationType;
		id = lookahead.getAttributeValue();
		
        if(match(sc->ID, syncSet)) {
		
			// Start dec-tail production rule
			if(lookahead.getTokenType() == sc->LPAREN) {
				dNode = transition("funDecTail", &Parser::funDecTail, syncSet);
				((ASTFunctionNode *)dNode)->declarationType = decType;
				((ASTFunctionNode *)dNode)->id = id;
			}
			else if(lookahead.getTokenType() == sc->LSQR ||
					lookahead.getTokenType() == sc->SEMI ||
					lookahead.getTokenType() == sc->COMMA) {
				dNode = transition("varDecTail", &Parser::varDecTail, syncSet);
				((ASTVariableDeclarationNode *)dNode)->declarationType = decType;
				((ASTVariableDeclarationNode *)dNode)->id = id;
			}
			else {
				syntaxError("dec-tail", syncSet);
			}
			// End dec-tail production rule
		}
    }
	else {
		syntaxError("declaration", syncSet);
	}
	
	return dNode;
}
//method for nonvoid-specifier production rule
ASTNode * Parser::nonVoidSpecifier(vector<int> syncSet){
	ASTDeclarationNode * dNode = new ASTDeclarationNode;
        dNode->lineNumber = admin->getLineNumber();
	
    if(lookahead.getTokenType() == sc->INT) {
        if(match(sc->INT, syncSet)) {
			dNode->declarationType = sc->INT;
		}
	}
    else if(lookahead.getTokenType() == sc->BOOL) {
        if(match(sc->BOOL, syncSet)) {
			dNode->declarationType = sc->BOOL;
		}
	}
	else {
		syntaxError("non-void-specifier", syncSet);
	}
	
	return dNode;
}

//method for var-dec-tail production rule
ASTNode * Parser::varDecTail(vector<int> syncSet){
	ASTVariableDeclarationNode * parent = new ASTVariableDeclarationNode, *vNode = parent;
        parent->lineNumber = admin->getLineNumber();
	/*vector<int> firstSet;
	
	firstSet.push_back(sc->LSQR); firstSet.push_back(sc->SEMI); firstSet.push_back(sc->COMMA);
	firstSet.insert(firstSet.end(), syncSet.begin(), syncSet.end());
	syntaxCheck(firstSet);*/
	
    if(lookahead.getTokenType() == sc->LSQR) {
		vector<int> syncSet2 = SyncSetBuilder::getSyncSet("[add-exp]");
		
		syncSet2.insert(syncSet2.end(), syncSet.begin(), syncSet.end());
		
        if(match(sc->LSQR, syncSet2)) {
		
			parent->arrayExp = dynamic_cast<ASTExpressionNode *>(transition("addExp", &Parser::addExp, syncSet2));
			
			if(match(sc->RSQR, syncSet2)) {

				parent->isArray = true;
			}
		}
    }
	
	/*firstSet.clear();
	firstSet.push_back(sc->SEMI); firstSet.push_back(sc->COMMA);
	firstSet.insert(firstSet.end(), syncSet.begin(), syncSet.end());
	syntaxCheck(firstSet);*/
	
    while(lookahead.getTokenType() == sc->COMMA){
		vector<int> syncSet2 = SyncSetBuilder::getSyncSet("{, var-name}");
		
		syncSet2.insert(syncSet2.end(), syncSet.begin(), syncSet.end());
		
		if(match(sc->COMMA, syncSet2)) {
			// The line below uses the first set because it is a repeating section
			vNode->next = transition("varName", &Parser::varName, syncSet2);
			vNode = ((ASTVariableDeclarationNode *)vNode->next);
		}
    }
    
    match(sc->SEMI, syncSet);
	
	return parent;
}

//method for var-name production rule
ASTNode * Parser::varName(vector<int> syncSet){
	int id = 0;
	ASTVariableDeclarationNode * vNode = new ASTVariableDeclarationNode;
        vNode->lineNumber = admin->getLineNumber();
	//vector<int> firstSet;
	
	id = lookahead.getAttributeValue();
    if(match(sc->ID, syncSet)) {
    
		/*firstSet.push_back(sc->LSQR);
		firstSet.insert(firstSet.end(), syncSet.begin(), syncSet.end());
		syntaxCheck(firstSet);*/

		if(lookahead.getTokenType() == sc->LSQR){
			vector<int> syncSet2 = SyncSetBuilder::getSyncSet("[add-exp]");

			syncSet2.insert(syncSet2.end(), syncSet.begin(), syncSet.end());

			if(match(sc->LSQR, syncSet2)) {
				
				vNode->arrayExp = dynamic_cast<ASTExpressionNode *>(transition("addExp", &Parser::addExp, syncSet2));
				if(match(sc->RSQR, syncSet2)) {

					vNode->isArray = true;
				}
			}
		}

		vNode->id = id;
	}
	
	return vNode;
}

//method for fun-dec-tail production rule
ASTNode * Parser::funDecTail(vector<int> syncSet){
	ASTFunctionNode * fNode = new ASTFunctionNode;
        fNode->lineNumber = admin->getLineNumber();
	ASTParamNode * pNode = new ASTParamNode;
	ASTCompoundNode * cNode = new ASTCompoundNode;
	
    if(match(sc->LPAREN, syncSet)) {
	
		pNode = ((ASTParamNode *)transition("params", &Parser::params, syncSet));

		if(match(sc->RPAREN, syncSet)) {

			cNode = dynamic_cast<ASTCompoundNode *>(transition("compoundStmt", &Parser::compoundStmt, syncSet));

			fNode->param = pNode;
			fNode->compound = cNode;
		}
	}
	
	return fNode;
}

//method for params production rule
ASTNode * Parser::params(vector<int> syncSet){
	ASTParamNode * parent = new ASTParamNode, *pNode = parent;
	//vector<int> firstSet;
	
    if(lookahead.getTokenType() == sc->REF ||
			lookahead.getTokenType() == sc->INT ||
			lookahead.getTokenType() == sc->BOOL) {
        parent = ((ASTParamNode *)transition("param", &Parser::param, syncSet));
		pNode = parent;
		
		/*firstSet.push_back(sc->COMMA);
		firstSet.insert(firstSet.end(), syncSet.begin(), syncSet.end());
		syntaxCheck(firstSet);*/
		
        while(lookahead.getTokenType() == sc->COMMA){
			vector<int> syncSet2 = SyncSetBuilder::getSyncSet("{, param}");

			syncSet2.insert(syncSet2.end(), syncSet.begin(), syncSet.end());
			
			if(match(sc->COMMA, syncSet2)) {
				pNode->next = transition("param", &Parser::param, syncSet2);
				pNode = ((ASTParamNode *)pNode->next);
			}
        }
    }
    else if(lookahead.getTokenType() == sc->VOID) {
        match(sc->VOID, syncSet);
    }
	else {
		syntaxError("params", syncSet);
	}
	
	return parent;
}

//method for param procution rule
ASTNode * Parser::param(vector<int> syncSet){
	ASTParamNode * pNode = new ASTParamNode;
        pNode -> lineNumber = admin->getLineNumber();
	ASTDeclarationNode * dNode = new ASTDeclarationNode;
	int id =0;
	//vector<int> firstSet;
	
    if(lookahead.getTokenType() == sc->REF){
		if(match(sc->REF, syncSet)) {
			dNode = ((ASTDeclarationNode *)transition("nonVoidSpecifier", &Parser::nonVoidSpecifier, syncSet));
			id = lookahead.getAttributeValue();
			if(match(sc->ID, syncSet)) {

				pNode->isRef = true;
				pNode->declarationType = dNode->declarationType;
				pNode->id = id;
			}
		}
    }
    else if(lookahead.getTokenType() == sc->INT ||
			lookahead.getTokenType() == sc->BOOL) {
        dNode = ((ASTDeclarationNode *)transition("nonVoidSpecifier", &Parser::nonVoidSpecifier, syncSet));
		id = lookahead.getAttributeValue();
		
        if(match(sc->ID, syncSet)) {
		
			/*firstSet.push_back(sc->LSQR);
			firstSet.insert(firstSet.end(), syncSet.begin(), syncSet.end());
			syntaxCheck(firstSet);*/

			if(lookahead.getTokenType() == sc->LSQR){
				vector<int> syncSet2 = SyncSetBuilder::getSyncSet("[]");
		
				syncSet2.insert(syncSet2.end(), syncSet.begin(), syncSet.end());

				if(match(sc->LSQR, syncSet2)) {

					if(match(sc->RSQR, syncSet2)) {

						pNode->isArray = true;
					}
				}
			}

			pNode->declarationType = dNode->declarationType;
			pNode->id = id;
		}
    }
	else {
		syntaxError("param", syncSet);
	}
	
	return pNode;
}

//method for statement production rule
ASTNode * Parser::statement(vector<int> syncSet){
	/* These substatements (exception: compoundStmt) do not need to have their sync
	 * sets assigned in their functions because they are only ever passed down
	 * by this statement function
	 */
    switch(lookahead.getTokenType()){
        case Scanner::ID:
			return transition("idStmt", &Parser::idStmt, syncSet);
            break;
        case Scanner::LCRLY:
			return transition("compoundStmt", &Parser::compoundStmt, syncSet);
            break;
        case Scanner::IF:
			return transition("ifStmt", &Parser::ifStmt, syncSet);
            break;
        case Scanner::LOOP:
			return transition("loopStmt", &Parser::loopStmt, syncSet);
            break;
        case Scanner::EXIT:
			return transition("exitStmt", &Parser::exitStmt, syncSet);
            break;
        case Scanner::CONTINUE:
			return transition("continueStmt", &Parser::continueStmt, syncSet);
            break;
        case Scanner::RETURN:
			return transition("returnStmt", &Parser::returnStmt, syncSet);
            break;
        case Scanner::BRANCH:
			return transition("branchStmt", &Parser::branchStmt, syncSet);
            break;
        case Scanner::SEMI:
			return transition("nullStmt", &Parser::nullStmt, syncSet);
            break;
		default:
			syntaxError("statement", syncSet);
			return new ASTStatementNode;
    }
}


//method for id-stmt production rule (including call-tail production rule)
/*idSmt contains the id-stmt produciton rule as well as call-tail production rule. 
 * This is so we could properly cast the Astnode to either ASTAssignmentnode or
 * ASTFunctionCall node, while providing the relevant information for the node
 */
ASTNode * Parser::idStmt(vector<int> syncSet){
	int id = lookahead.getAttributeValue();
	ASTStatementNode * sNode = new ASTStatementNode;
	
    if(match(sc->ID, syncSet)) {
	
		if(lookahead.getTokenType() == sc->LSQR || lookahead.getTokenType() == sc->ASSIGN){
			
			sNode = dynamic_cast<ASTStatementNode *>(transition("assignStmtTail", &Parser::assignStmtTail, syncSet));
			((ASTAssignmentNode *)sNode)->id = id;
		}
		else if(lookahead.getTokenType() == sc->LPAREN) {
			// Start call-tail production rule
			
			sNode = dynamic_cast<ASTStatementNode *>(transition("callTail", &Parser::callTail, syncSet));
			((ASTFunctionCallNode *)sNode)->id = id;
                        ((ASTFunctionCallNode *) sNode) ->isStatement = true;
			match(sc->SEMI, syncSet);
			// End call-tail production rule
		}
		else {
			syntaxError("id-stmt-tail", syncSet);
		}
	}
	
	return sNode;
}

//method for assign-stmt-tail production rule
ASTNode * Parser::assignStmtTail(vector<int> syncSet){
	ASTAssignmentNode * aNode = new ASTAssignmentNode;
        aNode->lineNumber=admin->getLineNumber();
	ASTExpressionNode * eNode = new ASTBinaryNode;
	/*vector<int> firstSet;
	
	firstSet.push_back(sc->LSQR); firstSet.push_back(sc->ASSIGN);
	firstSet.insert(firstSet.end(), syncSet.begin(), syncSet.end());
	syntaxCheck(firstSet);*/
	
    if(lookahead.getTokenType() == sc->LSQR){
		vector<int> syncSet2 = SyncSetBuilder::getSyncSet("[add-exp]");
		
		syncSet2.insert(syncSet2.end(), syncSet.begin(), syncSet.end());
		
        if(match(sc->LSQR, syncSet2)) {
		
			aNode->arrayExp = dynamic_cast<ASTExpressionNode *>(transition("addExp", &Parser::addExp, syncSet2));
			if(match(sc->RSQR, syncSet2)) {

				aNode->isArray = true;
			}
		}
    }
    
    if(match(sc->ASSIGN, syncSet)) {
	
		eNode = dynamic_cast<ASTExpressionNode *>(transition("expression", &Parser::expression, syncSet));
		if(match(sc->SEMI, syncSet)) {

			aNode->exp = eNode;
		}
	}
	
	return aNode;
}

//method for call-tail production rule
ASTNode * Parser::callTail(vector<int> syncSet){
	ASTFunctionCallNode * fNode = new ASTFunctionCallNode;
        fNode->lineNumber = admin->getLineNumber();
	ASTExpressionNode * argument = new ASTBinaryNode;
	//vector<int> firstSet;
	
    if(match(sc->LPAREN, syncSet)) {
    
		/*firstSet.push_back(sc->MINUS); firstSet.push_back(sc->NOT); firstSet.push_back(sc->LPAREN); firstSet.push_back(sc->NUM);
		firstSet.push_back(sc->BLIT); firstSet.push_back(sc->ID); firstSet.push_back(sc->RPAREN);
		firstSet.insert(firstSet.end(), syncSet.begin(), syncSet.end());
		syntaxCheck(firstSet);*/

		if(isExpressionLookahead()){
			vector<int> syncSet2 = SyncSetBuilder::getSyncSet("[arguments]");

			syncSet2.insert(syncSet2.end(), syncSet.begin(), syncSet.end());
			
			argument = dynamic_cast<ASTExpressionNode *>(transition("arguments", &Parser::arguments, syncSet));
			fNode->argument = argument;
		}

		match(sc->RPAREN, syncSet);
	}
	
	return ((ASTExpressionNode *)fNode);
}

//method for arguments production rule
ASTNode * Parser::arguments(vector<int> syncSet){
	ASTExpressionNode * parent = new ASTBinaryNode, *eNode = parent;
	//vector<int> firstSet;
	
	parent = dynamic_cast<ASTExpressionNode *>(transition("expression", &Parser::expression, syncSet));
	eNode = parent;
	
	/*firstSet.push_back(sc->MINUS); firstSet.push_back(sc->NOT); firstSet.push_back(sc->LPAREN); firstSet.push_back(sc->NUM);
	firstSet.push_back(sc->BLIT); firstSet.push_back(sc->ID);
	firstSet.insert(firstSet.end(), syncSet.begin(), syncSet.end());
	syntaxCheck(firstSet);*/
	
    while(lookahead.getTokenType() == sc->COMMA){
		vector<int> syncSet2 = SyncSetBuilder::getSyncSet("{, expression}");

		syncSet2.insert(syncSet2.end(), syncSet.begin(), syncSet.end());
			
        if(match(sc->COMMA, syncSet2)) {
			eNode->next = transition("expression", &Parser::expression, syncSet2);

			eNode = dynamic_cast<ASTExpressionNode *>(eNode->next);
		}
    }
	
	return parent;
}

//method for compound-stm production rule
ASTNode * Parser::compoundStmt(vector<int> syncSet){
	int decType = 0;
	int id =0;
	ASTCompoundNode * cNode = new ASTCompoundNode;
        cNode->lineNumber =admin->getLineNumber();
	ASTDeclarationNode * dNode = new ASTDeclarationNode;
	ASTStatementNode * sNode = new ASTStatementNode;
	ASTNode * current = cNode;
	
    if(match(sc->LCRLY, syncSet)) {
	
	//syntaxCheck(syncSet);
	
		while(lookahead.getTokenType() == sc->INT || lookahead.getTokenType() == sc->BOOL){
			vector<int> syncSet2 = SyncSetBuilder::getSyncSet("{nonvoid-specifier ID var-dec-tail}");

			syncSet2.insert(syncSet2.end(), syncSet.begin(), syncSet.end());
			
			dNode = ((ASTDeclarationNode *)transition("nonVoidSpecifier", &Parser::nonVoidSpecifier, syncSet2));
			id = lookahead.getAttributeValue();

			if(match(sc->ID, syncSet2)) {

				decType = dNode->declarationType;

				dNode = ((ASTDeclarationNode *)transition("varDecTail", &Parser::varDecTail, syncSet2));
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
		}

		do{
			
			sNode = dynamic_cast<ASTStatementNode *>(transition("statement", &Parser::statement, syncSet));

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

		match(sc->RCRLY, syncSet);
	}
	
	return cNode;
}

//method for if-stmt production rule
ASTNode * Parser::ifStmt(vector<int> syncSet){
	ASTIfNode * iNode = new ASTIfNode;
        iNode->lineNumber = admin->getLineNumber();
	
    if(match(sc->IF, syncSet)) {
	
		if(match(sc->LPAREN, syncSet)) {

			iNode->exp = dynamic_cast<ASTExpressionNode *>(transition("expression", &Parser::expression, syncSet));

			if(match(sc->RPAREN, syncSet)) {

				iNode->statement = dynamic_cast<ASTStatementNode *>(transition("statement", &Parser::statement, syncSet));

				//syntaxCheck(syncSet);

				if(lookahead.getTokenType() == sc->ELSE){
					vector<int> syncSet2 = SyncSetBuilder::getSyncSet("[else statement]");

					syncSet2.insert(syncSet2.end(), syncSet.begin(), syncSet.end());
					if(match(sc->ELSE, syncSet2)) {
						iNode->elseStatement = dynamic_cast<ASTStatementNode *>(transition("statement", &Parser::statement, syncSet2));
					}
				}
			}
		}
	}
	
	return iNode;
}

//method for loop-stmt production rule
ASTNode * Parser::loopStmt(vector<int> syncSet){
	ASTLoopNode * lNode = new ASTLoopNode;
	ASTNode * current = lNode;
    lNode->lineNumber = admin->getLineNumber();
	
	// For supporting semantics for exit/continue
	loopNesting++;
	
    if(match(sc->LOOP, syncSet)) {
	
		lNode->statement = dynamic_cast<ASTStatementNode *>(transition("statement", &Parser::statement, syncSet));
		current = lNode->statement;
		while(isStatementLookahead()) {
			current->next = transition("statement", &Parser::statement, syncSet);
			current = current->next;
		}
		if(match(sc->END, syncSet)) {
			match(sc->SEMI, syncSet);
		}
	}
	
	loopNesting--;
	
	return lNode;
}

//method for exit-stmt production rule
ASTNode * Parser::exitStmt(vector<int> syncSet){
	ASTMarkerNode * marker = new ASTMarkerNode;
	marker->lineNumber = admin->getLineNumber();
	marker->type = sc->EXIT;
    if(match(sc->EXIT, syncSet)) {
		match(sc->SEMI, syncSet);
	}
	
	if(loopNesting > 0) {
		marker->enabled = true;
	}
	
	return marker;
}

//method for continue-stmt production rule
ASTNode * Parser::continueStmt(vector<int> syncSet){
    ASTMarkerNode * marker = new ASTMarkerNode;
    marker->lineNumber = admin->getLineNumber();
    marker ->type = sc->CONTINUE;
    
    if(match(sc->CONTINUE, syncSet)) {
		match(sc->SEMI, syncSet);
	}
	
	if(loopNesting > 0) {
		marker->enabled = true;
	}
    
    return marker;
}

//method for return-stmt production rule
ASTNode * Parser::returnStmt(vector<int> syncSet){
    ASTReturnNode * rNode = new ASTReturnNode;
    rNode->lineNumber = admin->getLineNumber();
    if(match(sc->RETURN, syncSet)) {
	
		if(isExpressionLookahead()){
			vector<int> syncSet2 = SyncSetBuilder::getSyncSet("[expression]");

			syncSet2.insert(syncSet2.end(), syncSet.begin(), syncSet.end());

			rNode->expression = dynamic_cast<ASTExpressionNode *>(transition("expression", &Parser::expression, syncSet2));
		}

		match(sc->SEMI, syncSet);
	}
    return rNode;
}

//method for null-stmt production rule
ASTNode * Parser::nullStmt(vector<int> syncSet){   
    ASTMarkerNode * marker = new ASTMarkerNode;
    marker->lineNumber = admin->getLineNumber();
    marker ->type = sc->NULLSTMT;
    match(sc->SEMI, syncSet);
    
    return marker;
}

//method for branch-stmt production rule
ASTNode * Parser::branchStmt(vector<int> syncSet){
    ASTBranchNode * bNode = new ASTBranchNode;
    ASTCaseNode *current = NULL;
    bNode->lineNumber = admin->getLineNumber();
    
    if(match(sc->BRANCH, syncSet)) {
	
		if(match(sc->LPAREN, syncSet)) {

			bNode->expression = dynamic_cast<ASTExpressionNode *>(transition("addExp", &Parser::addExp, syncSet));

			if(match(sc->RPAREN, syncSet)) {

				bNode->firstCase = dynamic_cast<ASTCaseNode *>(transition("caseStmt", &Parser::caseStmt, syncSet));
				current =bNode->firstCase;
				while(lookahead.getTokenType() == sc->CASE || lookahead.getTokenType() == sc->DEFAULT){
					current->nextCase = dynamic_cast<ASTCaseNode *>(transition("caseStmt", &Parser::caseStmt, syncSet));
					current = current->nextCase;
				}

				if(match(sc->END, syncSet)) {
					match(sc->SEMI, syncSet);
				}
			}
		}
	}
    
    return bNode;
}

//method for case-stmt production rule
ASTNode * Parser::caseStmt(vector<int> syncSet){
    ASTCaseNode * cNode = new ASTCaseNode;
    cNode->lineNumber = admin->getLineNumber();
    if(lookahead.getTokenType() == sc->CASE){
        cNode->type = sc->CASE;
		
        if(match(sc->CASE, syncSet)) {
		
			cNode->num = lookahead.getAttributeValue();

			if(match(sc->NUM, syncSet)) {

				if(match(sc->COLON, syncSet)) {

					cNode->statement = dynamic_cast<ASTStatementNode *>(transition("statement", &Parser::statement, syncSet));
				}
			}
		}
    }
    else if(lookahead.getTokenType() == sc->DEFAULT) {
        cNode->type = sc->DEFAULT;
        
        if(match(sc->DEFAULT, syncSet)) {
		
			if(match(sc->COLON, syncSet)) {

				cNode->statement = dynamic_cast<ASTStatementNode *>(transition("statement", &Parser::statement, syncSet));
			}
		}
    }
	else {
		syntaxError("caseStmt", syncSet);
	}
    
    return cNode;
}

//method for expression production rule
// Commented out expression types because they will be taken care of later during semantic analysis
ASTNode * Parser::expression(vector<int> syncSet){
	//vector<int> firstSet;
	
    ASTExpressionNode * exp = dynamic_cast<ASTExpressionNode *>(transition("addExp", &Parser::addExp, syncSet));
	
	/*firstSet.push_back(sc->LTEQ); firstSet.push_back(sc->LT); firstSet.push_back(sc->GT); firstSet.push_back(sc->GTEQ);
	firstSet.push_back(sc->EQ); firstSet.push_back(sc->NEQ);
	firstSet.insert(firstSet.end(), syncSet.begin(), syncSet.end());
	syntaxCheck(firstSet);*/
	
    if(isRelopLookahead())
    {
		vector<int> syncSet2 = SyncSetBuilder::getSyncSet("[relop add-exp]");

		syncSet2.insert(syncSet2.end(), syncSet.begin(), syncSet.end());
			
		ASTBinaryNode * next = new ASTBinaryNode;
        next->oper = lookahead.getTokenType();
		
		if(match(lookahead.getTokenType(), syncSet2)) {
		
			next->right = dynamic_cast<ASTExpressionNode *>(transition("addExp", &Parser::addExp, syncSet2));
			next->left = exp;
			//next->type = ((ASTExpressionNode *)right)->type;
			exp = next;
		}
    }
	
	return exp;
}

//method for add-exp production rule (containing uminus and addop production rules )
/*
 */
ASTNode * Parser::addExp(vector<int> syncSet){
	bool isNeg = false;
	ASTExpressionNode * exp = NULL;
	/*vector<int> firstSet;
	
	firstSet.push_back(sc->MINUS);
	firstSet.insert(firstSet.end(), syncSet.begin(), syncSet.end());
	syntaxCheck(firstSet);*/
	
    if(lookahead.getTokenType() == sc->MINUS) {
		vector<int> syncSet2 = SyncSetBuilder::getSyncSet("[uminus]");

		syncSet2.insert(syncSet2.end(), syncSet.begin(), syncSet.end());
		
        //start uMinus production rule
        if(match(sc->MINUS, syncSet2)) {
        //end uminus production rule
			isNeg = true;
		}
    }
	
	exp = dynamic_cast<ASTExpressionNode *>(transition("term", &Parser::term, syncSet));
	
	if(isNeg) {
		ASTUnaryNode * next = new ASTUnaryNode;
		next->operation = sc->MINUS;
		next->operand = exp;
		//next->type = ((ASTExpressionNode *)exp)->type;
		exp = next;
	}
	
	/*firstSet.clear();
	firstSet.push_back(sc->PLUS); firstSet.push_back(sc->MINUS);
	firstSet.push_back(sc->OR); firstSet.push_back(sc->ORELSE);
	firstSet.insert(firstSet.end(), syncSet.begin(), syncSet.end());
	syntaxCheck(firstSet);*/
	
    while(isAddopLookahead()){
		vector<int> syncSet2 = SyncSetBuilder::getSyncSet("{addop term}");

		syncSet2.insert(syncSet2.end(), syncSet.begin(), syncSet.end());
		
		ASTBinaryNode * next = new ASTBinaryNode;
		next->oper = lookahead.getTokenType();
		
		if(match(lookahead.getTokenType(), syncSet2)) {
		
			next->right = dynamic_cast<ASTExpressionNode *>(transition("term", &Parser::term, syncSet2));
			next->left = exp;
			//next->type = ((ASTExpressionNode *)right)->type;
			exp = next;
		}
    }
	
	return exp;
}

//method for term production rule
ASTNode * Parser::term(vector<int> syncSet){
	//vector<int> firstSet;
	
    ASTExpressionNode * exp = dynamic_cast<ASTExpressionNode *>(transition("factor", &Parser::factor, syncSet));
	
	/*firstSet.push_back(sc->MULT); firstSet.push_back(sc->DIV); firstSet.push_back(sc->MOD);
	firstSet.push_back(sc->AND); firstSet.push_back(sc->ANDTHEN);
	firstSet.insert(firstSet.end(), syncSet.begin(), syncSet.end());
	syntaxCheck(firstSet);*/
	
    while(isMultopLookahead()) {
		vector<int> syncSet2 = SyncSetBuilder::getSyncSet("{multop factor}");

		syncSet2.insert(syncSet2.end(), syncSet.begin(), syncSet.end());
		
        ASTBinaryNode * next = new ASTBinaryNode;
        next->oper = lookahead.getTokenType();
		
		if(match(lookahead.getTokenType(), syncSet2)) {
		
			next->right = dynamic_cast<ASTExpressionNode *>(transition("factor", &Parser::factor, syncSet2));
			next->left = exp;
			//next->type = ((ASTExpressionNode *)right)->type;
			exp = next;
		}
    }
	
	return exp;
}

//method for factor production rule
ASTNode * Parser::factor(vector<int> syncSet){
	
    if(lookahead.getTokenType() == sc->ID){
        return transition("idFactor", &Parser::idFactor, syncSet);
    }
    else if(lookahead.getTokenType() == sc->NOT ||
			lookahead.getTokenType() == sc->LPAREN ||
			lookahead.getTokenType() == sc->NUM ||
			lookahead.getTokenType() == sc->BLIT) {
        return transition("nidFactor", &Parser::nidFactor, syncSet);
    }
	else {
		syntaxError("factor", syncSet);
		return NULL;
	}
}

//method for nid-factor production rule
ASTNode * Parser::nidFactor(vector<int> syncSet){
	ASTExpressionNode * eNode = NULL;
    switch(lookahead.getTokenType()){
        case Scanner::NOT:
			eNode = new ASTUnaryNode;
                        eNode->lineNumber = admin->getLineNumber();
            if(match(sc->NOT, syncSet)) {
				((ASTUnaryNode *)eNode)->operand = dynamic_cast<ASTExpressionNode *>(transition("factor", &Parser::factor, syncSet));
				((ASTUnaryNode *)eNode)->operation = sc->NOT;
				((ASTUnaryNode *)eNode)->type = ((ASTUnaryNode *)eNode)->operand->type;
			}
            break;
        case Scanner::LPAREN:
			
			if(match(sc->LPAREN, syncSet)) {
			
				eNode = dynamic_cast<ASTExpressionNode *>(transition("expression", &Parser::expression, syncSet));
				match(sc->RPAREN, syncSet);
			}
            break;
        case Scanner::NUM:
			eNode = new ASTLiteralNode;
                        eNode->lineNumber = admin->getLineNumber();
			eNode->type = sc->NUM;
			((ASTLiteralNode *)eNode)->value = lookahead.getAttributeValue();
            match(sc->NUM, syncSet);
            break;
        case Scanner::BLIT:
			eNode = new ASTLiteralNode;
                        eNode->lineNumber = admin->getLineNumber();
			eNode->type = sc->BLIT;
			((ASTLiteralNode *)eNode)->value = lookahead.getAttributeValue();
            match(sc->BLIT, syncSet);
            break;
		default:
                    // should never get called. left in for completeness of pattern
			syntaxError("nid-factor", syncSet);
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
ASTNode * Parser::idFactor(vector<int> syncSet){
	int id = lookahead.getAttributeValue();
	ASTExpressionNode * exp = NULL;
	//vector<int> firstSet;
	
    if(match(sc->ID, syncSet)) {
    
		// Start id-tail production rule
		if(lookahead.getTokenType() == sc->LPAREN){
			exp = dynamic_cast<ASTExpressionNode *>(transition("callTail", &Parser::callTail, syncSet));
			((ASTFunctionCallNode *)exp)->id = id;
		}
		else {
			exp = new ASTVariableNode;
                        exp->lineNumber = admin->getLineNumber();
			((ASTVariableNode *)exp)->id = id;

			/*firstSet.push_back(sc->LSQR);
			firstSet.insert(firstSet.end(), syncSet.begin(), syncSet.end());
			syntaxCheck(firstSet);*/

			// Start var-tail production rule
			if(lookahead.getTokenType() == sc->LSQR){
				vector<int> syncSet2 = SyncSetBuilder::getSyncSet("[add-exp]");

				syncSet2.insert(syncSet2.end(), syncSet.begin(), syncSet.end());

				if(match(sc->LSQR, syncSet2)) {

					((ASTVariableNode *)exp)->arrayExp = dynamic_cast<ASTExpressionNode *>(transition("addExp", &Parser::addExp, syncSet2));
					match(sc->RSQR, syncSet2);

					((ASTVariableNode *)exp)->isArray = true;
				}
			}
			// End var-tail production rule
		}
		// End id-tail production rule
	}
	
	return exp;
}

/*matches terminal symbols by checking if the lookahead is equal to the expected
 * terminal symbol. If so, it gets the next token as the new lookahead and sends
 * match and load information to the admin for traceing. if not in
 * sends error information to the admin for logging.
 */

bool Parser::match(int expected, vector<int> syncSet) {
    if(lookahead.getTokenType() == expected) {
		admin->parserLog(expected, PARSER_MATCH);
        lookahead = sc->getToken();
		admin->vec.push_back(lookahead);
		admin->scannerLogEnd();
		admin->parserLog(lookahead.getTokenType(), PARSER_LOAD);
		
		//syntaxCheck(syncSet);
		return true;
    }
    else {
       syntaxError(expected, syncSet);
	   return false;
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

/*void Parser::syntaxCheck(vector<int> syncSet) {
	if(find(syncSet.begin(), syncSet.end(), lookahead.getTokenType()) == syncSet.end()) {
		syntaxError(syncSet);
	}
}*/

void Parser::syntaxError(int expected, vector<int> syncSet) {
	admin->syntaxError(Scanner::namesRev[expected], lookahead.getTokenType());
	
	errorCount++;
	
	while(find(syncSet.begin(), syncSet.end(), lookahead.getTokenType()) == syncSet.end()) {
		lookahead = sc->getToken();
		admin->vec.push_back(lookahead);
		admin->scannerLogEnd();
	}
}
void Parser::syntaxError(string expected, vector<int> syncSet) {
	admin->syntaxError(expected, lookahead.getTokenType());
	
	errorCount++;
	
	while(find(syncSet.begin(), syncSet.end(), lookahead.getTokenType()) == syncSet.end()) {
		lookahead = sc->getToken();
		admin->vec.push_back(lookahead);
		admin->scannerLogEnd();
	}
}