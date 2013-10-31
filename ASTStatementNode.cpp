/* 
 * File:   ASTStatementNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:43 PM
 */

#include "ASTStatementNode.h"

ASTStatementNode::ASTStatementNode() : ASTNode() {
}

ASTStatementNode::ASTStatementNode(const ASTStatementNode& orig) : ASTNode(orig){
}

ASTStatementNode& ASTStatementNode::operator= (const ASTStatementNode &rhs)
{
	ASTNode::operator=(rhs);
	
    // return the existing object
    return *this;
}

ASTStatementNode::~ASTStatementNode() {
}

void ASTStatementNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    if(this->next != NULL)
        this->next->semAnalyze();
    //this->typeAnalyze();
    
}

void ASTStatementNode::scopeAnalyze(){
    
    
}

bool ASTStatementNode::returnAnalyze() {
	if(next != NULL) {
		return dynamic_cast<ASTStatementNode *>(next)->returnAnalyze();
	}
	
	return false;
}

void ASTStatementNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	printIndented("statement", indent);
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}