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

ASTVariableDeclarationNode::ASTVariableDeclarationNode(const ASTVariableDeclarationNode& orig) {
}

ASTVariableDeclarationNode::~ASTVariableDeclarationNode() {
}

void ASTVariableDeclarationNode::printNode(int indent) {
	ostringstream oss;
	
	printIndented("var dec:", indent);
	printIndented("name: " + idName, indent);
	printIndented("type: " + Scanner::namesRev[declarationType], indent);
	oss << "arrayParam? " << (isArray ? "YES" : "NO");
	printIndented(oss.str(), indent);
	
	if(isArray) {
		printIndented("size:", indent);
		arrayExp->printNode(indent + 2);
	}
	
	if(next != NULL) {
		next->printNode(indent);
	}
}