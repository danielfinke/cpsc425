/* 
 * File:   ASTCompoundNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:42 PM
 */

#include "ASTCompoundNode.h"

ASTCompoundNode::ASTCompoundNode() : ASTStatementNode(), dec(NULL), statement(NULL) {
}

ASTCompoundNode::ASTCompoundNode(const ASTCompoundNode& orig):ASTStatementNode(orig),
		dec(orig.dec), statement(orig.statement)
{
}

ASTCompoundNode& ASTCompoundNode::operator= (const ASTCompoundNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
	dec = rhs.dec;
	statement = rhs.statement;
 
    // return the existing object
    return *this;
}

ASTCompoundNode::~ASTCompoundNode() {
	delete dec;
	delete statement;
}

void ASTCompoundNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    this->dec->semAnalyze();
    this->statement->semAnalyze();
    
     if(this->next != NULL)
        this->next->semAnalyze();
    
    //this->typeAnalyze();
    
}

void ASTCompoundNode::scopeAnalyze(){
    
    
}

void ASTCompoundNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	if(dec != NULL) {
		printIndented("declarations", indent);
		dec->printNode(indent + 2, output);
	}
	
	if(statement != NULL) {
		printIndented("statements", indent);
		statement->printNode(indent + 2, output);
	}
	
	if(next != NULL) {
		next->printNode(indent, output);
	}	
}