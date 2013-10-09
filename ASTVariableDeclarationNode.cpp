/* 
 * File:   ASTVariableNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:46 PM
 */

#include "ASTVariableDeclarationNode.h"

ASTVariableDeclarationNode::ASTVariableDeclarationNode() : ASTDeclarationNode(), isArray(false), arrayExp(NULL)
{
}

ASTVariableDeclarationNode::ASTVariableDeclarationNode(const ASTVariableDeclarationNode& orig) :ASTDeclarationNode(orig),
        isArray(orig.isArray),arrayExp(orig.arrayExp)
{
}

ASTVariableDeclarationNode& ASTVariableDeclarationNode::operator= (const ASTVariableDeclarationNode &rhs)
{
	ASTDeclarationNode::operator=(rhs);
	
    // do the copy
        isArray = rhs.isArray;
        arrayExp = rhs.arrayExp;
 
    // return the existing object
    return *this;
}

ASTVariableDeclarationNode::~ASTVariableDeclarationNode() {
    delete arrayExp;
}

void ASTVariableDeclarationNode::printNode(int indent) {
	ostringstream oss;
	
	printIndented("var declaration", indent);
	printIndented("name: " + idName, indent + 2);
	printIndented("type: " + Scanner::namesRev[declarationType], indent + 2);
	oss << "arrayParam? " << (isArray ? "YES" : "NO");
	printIndented(oss.str(), indent + 2);
	
	if(isArray) {
		printIndented("size:", indent + 2);
		arrayExp->printNode(indent + 4);
	}
	
	if(next != NULL) {
		next->printNode(indent);
	}
}