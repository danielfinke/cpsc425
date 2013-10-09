/* 
 * File:   ASTDeclarationNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:43 PM
 */

#include "ASTDeclarationNode.h"

ASTDeclarationNode::ASTDeclarationNode() : ASTNode(), declarationType(0), id(0) {
}

ASTDeclarationNode::ASTDeclarationNode(const ASTDeclarationNode& orig):ASTNode(orig),
        declarationType(orig.declarationType), id(orig.id)
{
}


ASTDeclarationNode& ASTDeclarationNode::operator= (const ASTDeclarationNode &rhs)
{
	ASTNode::operator=(rhs);
	
    // do the copy
        declarationType= rhs.declarationType;
        id=rhs.id;
 
    // return the existing object
    return *this;
}

ASTDeclarationNode::~ASTDeclarationNode() {
}

void ASTDeclarationNode::printNode(int indent) {
	if(next != NULL) {
		next->printNode(indent);
	}	
}