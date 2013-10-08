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
	ostringstream oss;
	
	printIndented("function:", indent);
	printIndented("name: " + idName, indent);
	oss << "type: " << Scanner::namesRev[declarationType];
	printIndented(oss.str(), indent);
	if(param != NULL) {
		printIndented("params:", indent);
		param->printNode(indent + 2);
	}
	printIndented("body:", indent);
	compound->printNode(indent + 2);
	
	if(next != NULL) {
		next->printNode(indent);
	}
}