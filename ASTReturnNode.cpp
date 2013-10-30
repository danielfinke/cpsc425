/* 
 * File:   ASTReturnNode.cpp
 * Author: claire
 * 
 * Created on October 9, 2013, 12:09 AM
 */

#include "ASTReturnNode.h"
#include "SemanticAnalyzer.h"

ASTReturnNode::ASTReturnNode(): ASTStatementNode(), expression(NULL),
		funcScope(NULL)
{
}

ASTReturnNode::ASTReturnNode(const ASTReturnNode& orig):ASTStatementNode(orig),
		expression(orig.expression), funcScope(orig.funcScope)
{
}

ASTReturnNode& ASTReturnNode::operator= (const ASTReturnNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
	expression = rhs.expression;
	funcScope = rhs.funcScope;
 
    // return the existing object
	return *this;
}

ASTReturnNode::~ASTReturnNode() {
    delete expression;
}

void ASTReturnNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    if(this->expression !=NULL)
          this->expression->semAnalyze();
	
	returnAnalyze();
    
     if(this->next != NULL)
        this->next->semAnalyze();
    
    //this->typeAnalyze();
    
}

void ASTReturnNode::scopeAnalyze(){
    
    
}

void ASTReturnNode::returnAnalyze() {
	if(funcScope == NULL) {
		//sa->semanticError("Use of return outside of function", lineNumber);
		// Do nothing
	}
	// Assumption: funcScope's type already determined
	else if(funcScope->declarationType == Scanner::VOID) {
		if(this->expression != NULL) {
			sa->semanticError("Returning value from void function", lineNumber);
		}
	}
	else {
		// Missing return
		if(this->expression == NULL) {
			sa->semanticError("Missing return value for " +
				Scanner::namesRev[funcScope->declarationType] +
				"-returning function", lineNumber);
		}
		else if(this->expression->type == -1) {
			// Do nothing, error already reported
		}
		// Incorrect type
		else if((this->expression->type != Scanner::NUM &&
				this->expression->type != Scanner::INT) &&
				funcScope->declarationType == Scanner::INT) {
			sa->semanticError("Incorrect return type for " +
				Scanner::namesRev[funcScope->declarationType] +
				"-returning function", lineNumber);
		}
		else if((this->expression->type != Scanner::BLIT &&
				this->expression->type != Scanner::BOOL) &&
				funcScope->declarationType == Scanner::BOOL) {
			sa->semanticError("Incorrect return type for " +
				Scanner::namesRev[funcScope->declarationType] +
				"-returning function", lineNumber);
		}
	}
}

void ASTReturnNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	printIndented("return", indent);
	if(expression != NULL) {
		expression->printNode(indent + 2, output);
	}
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}