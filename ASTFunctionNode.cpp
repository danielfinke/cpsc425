/* 
 * File:   ASTFunctionNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:47 PM
 */

#include "ASTFunctionNode.h"
#include "ScopeTable.h"

ASTFunctionNode::ASTFunctionNode() : ASTDeclarationNode(), param(NULL), compound(NULL) {
}

ASTFunctionNode::ASTFunctionNode(const ASTFunctionNode& orig):ASTDeclarationNode(orig),param(orig.param),
        compound(orig.compound)
{
}

ASTFunctionNode& ASTFunctionNode::operator= (const ASTFunctionNode &rhs)
{
	ASTDeclarationNode::operator=(rhs);
	
    // do the copy
        param = rhs.param;
        compound = rhs.compound;
 
    // return the existing object
    return *this;
}

ASTFunctionNode::~ASTFunctionNode() {
    delete param;
    delete compound;
}

void ASTFunctionNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    this->param->semAnalyze();
    this->compound->semAnalyze();
    
     if(this->next != NULL)
        this->next->semAnalyze();
    //this->typeAnalyze();
    
}

void ASTFunctionNode::scopeAnalyze(){
    
    ST->insertDeclaration(id, this);
}

void ASTFunctionNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	printIndented("function", indent);
	if(ASTNode::lookup != NULL) {
		printIndented("id: " + ASTNode::lookup->getIdentifierName(id), indent + 2);
	}
	printIndented("type: " + Scanner::namesRev[declarationType], indent + 2);
	if(param != NULL) {
		printIndented("params:", indent + 2);
		param->printNode(indent + 4, output);
	}
	printIndented("body:", indent + 2);
	compound->printNode(indent + 4, output);
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}

int ASTFunctionNode::getParamCount() {
	int count = 0;
	ASTParamNode * p = param;
	
	// VOID param
	if(p->declarationType == 0) {
		return 0;
	}
	
	while(p != NULL) {
		count++;
		p = dynamic_cast<ASTParamNode *>(p->next);
	}
	
	return count;
}