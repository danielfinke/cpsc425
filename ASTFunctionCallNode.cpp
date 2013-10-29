/* 
 * File:   ASTFunctionCallNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:40 PM
 */

#include "ASTFunctionCallNode.h"

ASTFunctionCallNode::ASTFunctionCallNode() : ASTStatementNode(), ASTExpressionNode(), id(0),
		funcDec(NULL), argument(NULL) {
}

ASTFunctionCallNode::ASTFunctionCallNode(const ASTFunctionCallNode& orig): ASTStatementNode(orig), 
        ASTExpressionNode(orig), id(orig.id), funcDec(orig.funcDec),
		argument(orig.argument)
{
}

ASTFunctionCallNode& ASTFunctionCallNode::operator= (const ASTFunctionCallNode &rhs)
{
	ASTStatementNode::operator=(rhs);
        ASTExpressionNode::operator = (rhs);
	
    // do the copy
	id = rhs.id;
	funcDec = rhs.funcDec;
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
    this->typeAnalyze();
    
}

void ASTFunctionCallNode::scopeAnalyze(){
    
    
}

void ASTFunctionCallNode::typeAnalyze() {
	if(funcDec == NULL) {
		// Throw exception
	}
	
	type = funcDec->declarationType;
	
	if(funcDec->getParamCount() != getArgCount()) {
		// Semantic error - incorrect number of parameters
		return;
	}
	
	// Check all of the params for correct types
	ASTExpressionNode * arg = argument;
	ASTParamNode * param = funcDec->param;
	while(arg != NULL && param != NULL) {
		if(arg->type != param->declarationType) {
			// Semantic error - mismatched argument types
		}
	}
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

int ASTFunctionCallNode::getArgCount() {
	int count = 0;
	ASTExpressionNode * arg = argument;
	
	while(arg != NULL) {
		count++;
		arg = arg->next;
	}
	
	return count;
}