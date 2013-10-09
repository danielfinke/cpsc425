/* 
 * File:   ASTDeclarationNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:43 PM
 */

#include "ASTDeclarationNode.h"

ASTDeclarationNode::ASTDeclarationNode() : ASTNode(), declarationType(0), idName("") {
}

ASTDeclarationNode::ASTDeclarationNode(const ASTDeclarationNode& orig) {
}

ASTDeclarationNode::~ASTDeclarationNode() {
}

void ASTDeclarationNode::printNode(int indent) {
	if(next != NULL) {
		next->printNode(indent);
	}	
}