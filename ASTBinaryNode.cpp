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
	printIndented("binop", indent);
	//printIndented("type: " + Scanner::namesRev[type], indent);
	printIndented("operation: " + Scanner::namesRev[oper], indent + 2);
	printIndented("left:", indent + 2);
	left->printNode(indent + 4);
	printIndented("right:", indent + 2);
	right->printNode(indent + 4);
	
	if(next != NULL) {
		next->printNode(indent);
	}
}