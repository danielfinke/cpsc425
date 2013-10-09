/* 
 * File:   ASTFunctionNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:47 PM
 */

#include "ASTFunctionNode.h"

ASTFunctionNode::ASTFunctionNode() : ASTDeclarationNode(), param(NULL), compound(NULL) {
}

ASTFunctionNode::ASTFunctionNode(const ASTFunctionNode& orig) {
}

ASTFunctionNode::~ASTFunctionNode() {
}

void ASTFunctionNode::printNode(int indent) {
	printIndented("function", indent);
	printIndented("id: " + idName, indent + 2);
	printIndented("type: " + Scanner::namesRev[declarationType], indent + 2);
	if(param != NULL) {
		printIndented("params:", indent + 2);
		param->printNode(indent + 4);
	}
	printIndented("body:", indent + 2);
	compound->printNode(indent + 4);
	
	if(next != NULL) {
		next->printNode(indent);
	}
}