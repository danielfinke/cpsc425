/* 
 * File:   ASTParamNode.cpp
 * Author: daniel
 * 
 * Created on October 7, 2013, 11:15 PM
 */

#include "ASTParamNode.h"

ASTParamNode::ASTParamNode() : ASTVariableDeclarationNode(), isRef(false) {
}

ASTParamNode::ASTParamNode(const ASTParamNode& orig) {
}

ASTParamNode::~ASTParamNode() {
}

void ASTParamNode::printNode(int indent) {
	printIndented("param:", indent);
	printIndented("name: " + idName, indent);
	printIndented("type: " + declarationType, indent);
	printIndented("ref? " + isRef ? "YES" : "NO", indent);
	printIndented("arrayParam? " + isArray ? "YES" : "NO", indent);
	
	if(next != NULL) {
		next->printNode(indent);
	}
}