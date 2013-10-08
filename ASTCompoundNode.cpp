/* 
 * File:   ASTCompoundNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:42 PM
 */

#include "ASTCompoundNode.h"

ASTCompoundNode::ASTCompoundNode() : ASTNode() {
}

ASTCompoundNode::ASTCompoundNode(const ASTCompoundNode& orig) {
}

ASTCompoundNode::~ASTCompoundNode() {
}

void ASTCompoundNode::printNode(int indent) {
	if(next != NULL) {
		next->printNode(indent);
	}	
}