/* 
 * File:   ASTIfNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 1:35 AM
 */

#include "ASTIfNode.h"
#include "SemanticAnalyzer.h"

ASTIfNode::ASTIfNode() : ASTStatementNode(), exp(NULL), statement(NULL), elseStatement(NULL) {
}

ASTIfNode::ASTIfNode(const ASTIfNode& orig) : ASTStatementNode(orig), exp(orig.exp),
        statement(orig.statement),elseStatement(orig.statement)
{
}

ASTIfNode& ASTIfNode::operator= (const ASTIfNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
        exp=rhs.exp;
        statement = rhs.statement;
        elseStatement = rhs.elseStatement;
    
 
    // return the existing object
    return *this;
}

ASTIfNode::~ASTIfNode() {
    delete exp;
    delete statement;
    delete elseStatement;
}

void ASTIfNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    this->exp->semAnalyze();
	
    this->typeAnalyze();
	
    this->statement->semAnalyze();
    
    if(elseStatement != NULL)
        this->elseStatement->semAnalyze();
    
    if(this->next != NULL)
        this->next->semAnalyze();
    
}

void ASTIfNode::scopeAnalyze(){
    
    
}

void ASTIfNode::typeAnalyze() {
	if(exp == NULL) {
		return;
	}
	
	if(exp->type != Scanner::BOOL) {
		// Semantic error - expression must be of boolean type
		sa->semanticError("Expression must be of boolean type", lineNumber);
	}
}

void ASTIfNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	printIndented("if", indent);
	printIndented("condition:", indent + 2);
	exp->printNode(indent + 4, output);
	printIndented("true branch:", indent + 2);
	statement->printNode(indent + 4, output);
	
	if(elseStatement != NULL) {
		printIndented("false branch:", indent + 2);
		elseStatement->printNode(indent + 4, output);
	}
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}