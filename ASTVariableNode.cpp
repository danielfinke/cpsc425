/* 
 * File:   ASTVariableNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 8:51 AM
 */

#include "ASTVariableNode.h"

ASTVariableNode::ASTVariableNode() : ASTExpressionNode(), id(0), isArray(false),
		arrayExp(NULL)
{
}

ASTVariableNode::ASTVariableNode(const ASTVariableNode& orig) : ASTExpressionNode(orig),
        id(orig.id), isArray(orig.isArray), arrayExp(orig.arrayExp)
{
}

ASTVariableNode& ASTVariableNode::operator= (const ASTVariableNode &rhs)
{
	ASTExpressionNode::operator=(rhs);
	
    // do the copy
        id = rhs.id;
        isArray = rhs.isArray;
        arrayExp = rhs.arrayExp;
 
    // return the existing object
    return *this;
}

ASTVariableNode::~ASTVariableNode() {
    delete arrayExp;
}

void ASTVariableNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    if(this->isArray)
        this->arrayExp->semAnalyze();
    
     if(this->next != NULL)
        this->next->semAnalyze();
    //this->typeAnalyze();
    
}

void ASTVariableNode::scopeAnalyze(){
    
    if(! ST->isInScope(this->id))
   {
       //throw scope error
   }
    
}

void ASTVariableNode::printNode(int indent, ostream * output) {
	ostringstream oss;
	
	this->output = output;
	
	printIndented("var", indent);
	if(ASTNode::lookup != NULL) {
		printIndented("id: " + ASTNode::lookup->getIdentifierName(id), indent + 2);
	}
	printIndented("type: " + Scanner::namesRev[type], indent + 2);
	oss << "array? " << (isArray ? "YES" : "NO");
	printIndented(oss.str(), indent + 2);
	
	if(isArray) {
		printIndented("index:", indent + 2);
		arrayExp->printNode(indent + 4, output);
	}
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}