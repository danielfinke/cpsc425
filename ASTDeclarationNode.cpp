/* 
 * File:   ASTDeclarationNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:43 PM
 */

#include "ASTDeclarationNode.h"

ASTDeclarationNode::ASTDeclarationNode() : ASTNode(), declarationType(0), idName("") {
}

ASTDeclarationNode::ASTDeclarationNode(const ASTDeclarationNode& orig):ASTNode(orig),
        declarationType(orig.declarationType), idName(orig.idName)
{
}


ASTDeclarationNode& ASTDeclarationNode::operator= (const ASTDeclarationNode &rhs)
{
	ASTNode::operator=(rhs);
	
    // do the copy
        declarationType= rhs.declarationType;
        idName=rhs.idName;
 
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