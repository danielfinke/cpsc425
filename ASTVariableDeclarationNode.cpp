/* 
 * File:   ASTVariableNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:46 PM
 */

#include "ASTVariableDeclarationNode.h"
#include "ScopeTable.h"
#include "ASTLiteralNode.h"

ASTVariableDeclarationNode::ASTVariableDeclarationNode() : ASTDeclarationNode(),
		level(0), displacement(0),
		isArray(false), arrayExp(NULL)
{
}

ASTVariableDeclarationNode::ASTVariableDeclarationNode(const ASTVariableDeclarationNode& orig) :ASTDeclarationNode(orig),
        level(orig.level), displacement(orig.displacement),
		isArray(orig.isArray),arrayExp(orig.arrayExp)
{
}

ASTVariableDeclarationNode& ASTVariableDeclarationNode::operator= (const ASTVariableDeclarationNode &rhs)
{
	ASTDeclarationNode::operator=(rhs);
	
    // do the copy
	level = rhs.level;
	displacement = rhs.displacement;
	isArray = rhs.isArray;
	arrayExp = rhs.arrayExp;
 
    // return the existing object
    return *this;
}

ASTVariableDeclarationNode::~ASTVariableDeclarationNode() {
    delete arrayExp;
}

void ASTVariableDeclarationNode::semAnalyze(){
    
    if(init || !isGlobalDec){
        this->scopeAnalyze();
        if(init) return;
    }
    
    if(isArray) {
		// Analyze with restriction that vars not allowed
        arrayExp->semAnalyze(true);
		// Check that the value is a positive number
		ASTLiteralNode * sizeVal = arrayExp->calc();
		if(sizeVal == NULL ||
				sizeVal->value < 0) {
			sa->semanticError("Array size must be a positive integer", lineNumber);
		}
	}
    
    if(this->next != NULL)
        this->next->semAnalyze();
    
    
    //this->typeAnalyze();
}

void ASTVariableDeclarationNode :: scopeAnalyze(){
    
    sa->getST()->insertDeclaration(this-> id, this);
    
}

void ASTVariableDeclarationNode::printNode(int indent, ostream * output) {
	ostringstream oss;
	
	this->output = output;
	
	printIndented("var declaration", indent);
	if(ASTNode::lookup != NULL) {
		printIndented("id: " + ASTNode::lookup->getIdentifierName(id), indent + 2);
	}
	printIndented("type: " + Scanner::namesRev[declarationType], indent + 2);
	oss << "arrayParam? " << (isArray ? "YES" : "NO");
	printIndented(oss.str(), indent + 2);
	
	if(isArray) {
		printIndented("size:", indent + 2);
		arrayExp->printNode(indent + 4, output);
	}
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}