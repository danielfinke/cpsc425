/* 
 * File:   ASTVariableNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 8:51 AM
 */

#include "ASTVariableNode.h"

ASTVariableNode::ASTVariableNode() : ASTExpressionNode(), idName(""), isArray(false),
		arrayExp(NULL), func(NULL)
{
}

ASTVariableNode::ASTVariableNode(const ASTVariableNode& orig) {
}

ASTVariableNode::~ASTVariableNode() {
}

void ASTVariableNode::printNode(int indent) {
	ostringstream oss;
	
	printIndented("var:", indent);
	printIndented("name: " + idName, indent);
	printIndented("type: " + Scanner::namesRev[type], indent);
	oss << "array? " << (isArray ? "YES" : "NO");
	printIndented(oss.str(), indent);
	
	if(isArray) {
		printIndented("index:", indent);
		arrayExp->printNode(indent + 2);
	}
	
	if(func != NULL) {
		printIndented("func:", indent);
		func->printNode(indent + 2);
	}
	
	if(next != NULL) {
		next->printNode(indent);
	}
}