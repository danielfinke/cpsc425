/* 
 * File:   ASTBinaryNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:47 PM
 */

#include "ASTBinaryNode.h"

ASTBinaryNode::ASTBinaryNode() : ASTExpressionNode(), oper(0), left(NULL), right(NULL) {
}

ASTBinaryNode::ASTBinaryNode(const ASTBinaryNode& orig) {
}

ASTBinaryNode::~ASTBinaryNode() {
}

void ASTBinaryNode::printNode(int indent) {
	printIndented("binary exp:", indent);
	printIndented("type: " + Scanner::namesRev[type], indent);
	printIndented("operation: " + Scanner::namesRev[oper], indent);
	printIndented("left:", indent);
	left->printNode(indent + 2);
	printIndented("right:", indent);
	right->printNode(indent + 2);
	
	if(next != NULL) {
		next->printNode(indent);
	}
}