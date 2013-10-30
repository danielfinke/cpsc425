/* 
 * File:   ASTCaseNode.cpp
 * Author: claire
 * 
 * Created on October 9, 2013, 12:09 AM
 */

#include "ASTCaseNode.h"
#include "SemanticAnalyzer.h"

ASTCaseNode::ASTCaseNode() : ASTStatementNode(),statement(NULL), nextCase(NULL), type(0), num(0) {
}

ASTCaseNode::ASTCaseNode(const ASTCaseNode& orig):ASTStatementNode(orig), statement(orig.statement),
        nextCase(orig.nextCase), type(orig.type), num(orig.num)
{
}

ASTCaseNode& ASTCaseNode::operator= (const ASTCaseNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
        statement = rhs.statement;
        nextCase = rhs.nextCase;
        type = rhs.type;
        num = rhs.num;
    
    // return the existing object
        return *this;
}

ASTCaseNode::~ASTCaseNode() {
    delete statement;
    delete nextCase;
}

void ASTCaseNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    this->statement->semAnalyze();
	if(nextCase != NULL) {
		this->nextCase->semAnalyze();
	}
    
     if(this->next != NULL)
        this->next->semAnalyze();
    this->typeAnalyze();
    
}

void ASTCaseNode::scopeAnalyze(){
    
    //nothing
}

void ASTCaseNode::typeAnalyze() {
	/*if(type != Scanner::NUM && type != Scanner::DEFAULT) {
		// Semantic error - expected NUM or DEFAULT
		sa->semanticError("Expected NUM or DEFAULT", lineNumber);
	}*/
}

void ASTCaseNode::printNode(int indent, ostream * output) {
	ostringstream oss;
	
	this->output = output;
	
	printIndented(Scanner::namesRev[type], indent);
	if(type != Scanner::DEFAULT) {
		oss << "value: " << num;
		printIndented(oss.str(), indent + 2);
	}
	
	if(statement != NULL) {
		printIndented("statements:" , indent + 2);
		statement->printNode(indent + 4, output);
	}
	
	if(nextCase != NULL) {
		nextCase->printNode(indent, output);
	}
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}