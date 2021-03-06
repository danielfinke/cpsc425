/* 
 * File:   ASTParamNode.cpp
 * Author: daniel
 * 
 * Created on October 7, 2013, 11:15 PM
 */

#include "ASTParamNode.h"
#include "ScopeTable.h"

ASTParamNode::ASTParamNode() : ASTVariableDeclarationNode(), isRef(false) {
}

ASTParamNode::ASTParamNode(const ASTParamNode& orig):ASTVariableDeclarationNode(orig),
        isRef(orig.isRef)
{
}

ASTParamNode& ASTParamNode::operator= (const ASTParamNode &rhs)
{
	ASTVariableDeclarationNode::operator=(rhs);
	
    // do the copy
        isRef = rhs.isRef;
    
 
    // return the existing object
    return *this;
}

ASTParamNode::~ASTParamNode() {
}

void ASTParamNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    /*if(this->isArray)
       this->arrayExp->semAnalyze();*/
    
    if(this->next != NULL)
        this->next->semAnalyze();
    //this->typeAnalyze();
    
}

void ASTParamNode::scopeAnalyze(){
    
    sa->getST()->insertDeclaration(id, this);
    
}

void ASTParamNode::printNode(int indent, ostream * output) {
	ostringstream oss;
	
	this->output = output;
	
	printIndented("param", indent);
	if(ASTNode::lookup != NULL) {
		printIndented("id: " + ASTNode::lookup->getIdentifierName(id), indent + 2);
	}
	printIndented("type: " + Scanner::namesRev[declarationType], indent + 2);
	oss << "ref? " << (isRef ? "YES" : "NO");
	printIndented(oss.str(), indent + 2);
	oss.str("");
	oss << "arrayParam? " << (isArray ? "YES" : "NO");
	printIndented(oss.str(), indent + 2);

	if(next != NULL) {
		next->printNode(indent, output);
	}
}