/* 
 * File:   ASTCompoundNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:42 PM
 */

#include "ASTCompoundNode.h"

ASTCompoundNode::ASTCompoundNode() : ASTNode() {
}

ASTCompoundNode::ASTCompoundNode(const ASTCompoundNode& orig):ASTNode(orig) {
}

ASTCompoundNode& ASTCompoundNode::operator= (const ASTCompoundNode &rhs)
{
	ASTNode::operator=(rhs);
 
    // return the existing object
    return *this;
}

ASTCompoundNode::~ASTCompoundNode() {
}

void ASTCompoundNode::printNode(int indent) {
	if(next != NULL) {
		next->printNode(indent);
	}	
}