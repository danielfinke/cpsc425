/* 
 * File:   ASTFunctionCallNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:40 PM
 */

#include "ASTFunctionCallNode.h"

ASTFunctionCallNode::ASTFunctionCallNode() : ASTStatementNode(), ASTExpressionNode(), id(0), argument(NULL) {
}

ASTFunctionCallNode::ASTFunctionCallNode(const ASTFunctionCallNode& orig): ASTStatementNode(orig), 
        ASTExpressionNode(orig), id(orig.id),argument(orig.argument)
{
}

ASTFunctionCallNode& ASTFunctionCallNode::operator= (const ASTFunctionCallNode &rhs)
{
	ASTStatementNode::operator=(rhs);
        ASTExpressionNode::operator = (rhs);
	
    // do the copy
        id= rhs.id;
        argument = rhs.argument;
 
    // return the existing object
    return *this;
}

ASTFunctionCallNode::~ASTFunctionCallNode() {
    delete argument;
}

void ASTFunctionCallNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    this->argument->semAnalyze();
    
     if(this->next != NULL)
        this->next->semAnalyze();
    //this->typeAnalyze();
    
}

void ASTFunctionCallNode::scopeAnalyze(){
    
    
}
void ASTFunctionCallNode::printNode(int indent, ostream * output) {
	this->ASTStatementNode::output = output;
	
	ASTStatementNode::printIndented("function call", indent);
	if(ASTNode::lookup != NULL) {
		ASTStatementNode::printIndented("id: " + ASTNode::lookup->getIdentifierName(id), indent + 2);
	}
	
	if(argument != NULL) {
		ASTStatementNode::printIndented("args:", indent + 2);
		argument->printNode(indent + 4, output);
	}
	
	if(ASTStatementNode::next != NULL) {
		ASTStatementNode::next->printNode(indent, output);
	}
}