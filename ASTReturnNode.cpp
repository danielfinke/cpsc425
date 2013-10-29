/* 
 * File:   ASTReturnNode.cpp
 * Author: claire
 * 
 * Created on October 9, 2013, 12:09 AM
 */

#include "ASTReturnNode.h"

ASTReturnNode::ASTReturnNode(): ASTStatementNode(), expression(NULL) {
}

ASTReturnNode::ASTReturnNode(const ASTReturnNode& orig):ASTStatementNode(orig),expression(orig.expression) {
}

ASTReturnNode& ASTReturnNode::operator= (const ASTReturnNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
        expression = rhs.expression;

 
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
    
     if(this->next != NULL)
        this->next->semAnalyze();
    
    //this->typeAnalyze();
    
}

void ASTReturnNode::scopeAnalyze(){
    
    
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