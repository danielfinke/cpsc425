/* 
 * File:   ASTFunctionNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:47 PM
 */

#include "ASTFunctionNode.h"
#include "ScopeTable.h"

ASTFunctionNode::ASTFunctionNode() : ASTDeclarationNode(), returnMet(false),
		param(NULL), compound(NULL) {
}

ASTFunctionNode::ASTFunctionNode(const ASTFunctionNode& orig):ASTDeclarationNode(orig),
		returnMet(orig.returnMet), param(orig.param),
        compound(orig.compound)
{
}

ASTFunctionNode& ASTFunctionNode::operator= (const ASTFunctionNode &rhs)
{
	ASTDeclarationNode::operator=(rhs);
	
    // do the copy
	returnMet = rhs.returnMet;
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
	
	if(getParamCount() > 0) {
		this->param->semAnalyze();
	}
    this->compound->semAnalyze();
	
	returnAnalyze();
    
     if(this->next != NULL)
        this->next->semAnalyze();
    //this->typeAnalyze();
    
}

void ASTFunctionNode::scopeAnalyze(){
    
    sa->getST()->insertDeclaration(id, this);
}

void ASTFunctionNode::returnAnalyze() {
	if(this->declarationType != Scanner::VOID && !returnMet) {
		sa->semanticError("Missing return statement for function: " +
			ASTNode::lookup->getIdentifierName(id), lineNumber);
	}
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