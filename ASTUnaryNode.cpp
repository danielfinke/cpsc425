/* 
 * File:   ASTUnaryNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:47 PM
 */

#include "ASTUnaryNode.h"

ASTUnaryNode::ASTUnaryNode() : ASTExpressionNode(), operation(0), type(0), operand(NULL) {
}

ASTUnaryNode::ASTUnaryNode(const ASTUnaryNode& orig) {
}

ASTUnaryNode::~ASTUnaryNode() {
}

void ASTUnaryNode::printNode(int indent) {
	printIndented("unary exp", indent);
	//printIndented("type: " + Scanner::namesRev[type], indent + 2);
	printIndented("operation: " + Scanner::namesRev[operation], indent + 2);
	printIndented("operand:", indent + 2);
	operand->printNode(indent + 4);
	
	if(next != NULL) {
		next->printNode(indent);
	}
}