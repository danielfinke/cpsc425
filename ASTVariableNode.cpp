/* 
 * File:   ASTVariableNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 8:51 AM
 */

#include "ASTVariableNode.h"
#include "ScopeTable.h"

ASTVariableNode::ASTVariableNode() : ASTExpressionNode(), id(0),
		varDec(NULL), isArray(false),
		arrayExp(NULL)
{
}

ASTVariableNode::ASTVariableNode(const ASTVariableNode& orig) : ASTExpressionNode(orig),
        id(orig.id), varDec(NULL),
		isArray(orig.isArray), arrayExp(orig.arrayExp)
{
}

ASTVariableNode& ASTVariableNode::operator= (const ASTVariableNode &rhs)
{
	ASTExpressionNode::operator=(rhs);
	
    // do the copy
	id = rhs.id;
	varDec = rhs.varDec;
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
     
    this->typeAnalyze();
	
	if(!varDec->initialized) {
		// Semantic error - uninitialized variable
		sa->semanticError("Variable has not been initialized", lineNumber);
	}
    
     if(this->next != NULL)
        this->next->semAnalyze();
    
}

void ASTVariableNode::semAnalyze(bool restrictIdents){
	
	if(restrictIdents) {
		sa->semanticError("Array size must be static", lineNumber);
		restrictIdents = false;
	}
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
	
    this->typeAnalyze();
    
    if(this->isArray)
        this->arrayExp->semAnalyze(restrictIdents);
    
     if(this->next != NULL)
        this->next->semAnalyze();
    
}

void ASTVariableNode::scopeAnalyze(){
    
	varDec = (ASTVariableDeclarationNode *)(sa->getST()->getDeclaration(id, lineNumber));
    
}

void ASTVariableNode::typeAnalyze() {
	if(varDec == NULL) {
		//throw "NULL in varDec";
		type = -1;
		return;
	}
	
	type = varDec->declarationType;
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