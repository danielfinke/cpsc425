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
	printIndented("unary exp:", indent);
	printIndented("type: " + type, indent);
	printIndented("operation: " + operation, indent);
	printIndented("operand:", indent);
	operand->printNode(indent + 2);
	
	if(next != NULL) {
		next->printNode(indent);
	}
}