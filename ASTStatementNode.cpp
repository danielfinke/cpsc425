/* 
 * File:   ASTStatementNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:43 PM
 */

#include "ASTStatementNode.h"

ASTStatementNode::ASTStatementNode() : ASTNode() {
}

ASTStatementNode::ASTStatementNode(const ASTStatementNode& orig) {
}

ASTStatementNode::~ASTStatementNode() {
}

void ASTStatementNode::printNode(int indent) {
	printIndented("statement:", indent);
	
	if(next != NULL) {
		next->printNode(indent);
	}
}