/* 
 * File:   ASTBranchNode.cpp
 * Author: claire
 * 
 * Created on October 9, 2013, 12:09 AM
 */

#include "ASTBranchNode.h"
#include "SemanticAnalyzer.h"

ASTBranchNode::ASTBranchNode(): ASTStatementNode(), expression(NULL), firstCase(NULL){
}

ASTBranchNode::ASTBranchNode(const ASTBranchNode& orig):ASTStatementNode(orig),
        expression(orig.expression), firstCase(orig.firstCase)
{
}

ASTBranchNode& ASTBranchNode::operator= (const ASTBranchNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
        expression = rhs.expression;
        firstCase = rhs.firstCase;
    
 
    // return the existing object
        return *this;
}
  

ASTBranchNode::~ASTBranchNode() {
    delete expression;
    delete firstCase;
}

void ASTBranchNode ::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    this->expression->semAnalyze();
    this-> firstCase->semAnalyze();
    this->typeAnalyze();
    
     if(this->next != NULL)
        this->next->semAnalyze();
    
    
}

void ASTBranchNode:: scopeAnalyze(){
    
    //nothing?
}

void ASTBranchNode::typeAnalyze() {
	if(expression == NULL) {
		throw "NULL in expression";
	}
	
	if(expression->type != Scanner::INT) {
		// Semantic error - integral expression expected
		sa->semanticError("Integral expression expected", lineNumber);
	}
}

void ASTBranchNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	printIndented("branch", indent);
	printIndented("exp:", indent + 2);
	expression->printNode(indent + 4, output);
	printIndented("cases:", indent + 2);
	firstCase->printNode(indent + 4, output);
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}