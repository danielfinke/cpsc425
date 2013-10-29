/* 
 * File:   ASTNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 11:47 AM
 */

#include "ASTNode.h"
#include "ScopeTable.h"

Scanner * ASTNode::lookup = 0;
ScopeTable * ASTNode::ST = 0;
bool ASTNode::init = true;

ASTNode::ASTNode() : next(NULL), output(NULL), isGlobalDec(false){
}

ASTNode::ASTNode(const ASTNode& orig) : next(orig.next), output(orig.output),
        isGlobalDec(orig.isGlobalDec)
{
}

ASTNode& ASTNode:: operator = (const ASTNode& rhs){
	next = rhs.next;
	output = rhs.output;
        isGlobalDec = rhs.isGlobalDec;
	
    return *this;
}

ASTNode::~ASTNode() {
    delete next;
}

void ASTNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
     if(this->next != NULL)
        this->next->semAnalyze();
    
    //this->typeAnalyze();
    
}

void ASTNode::scopeAnalyze(){
    
    
}

void ASTNode::printIndented(string text, int indent) {
	for(int i = 0; i < indent; i++) {
		*output << " ";
	}
	*output << text << endl;
}