/* 
 * File:   ASTLiteralNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 9:08 AM
 */

#include "ASTLiteralNode.h"

ASTLiteralNode::ASTLiteralNode() : ASTExpressionNode(), value(0) {
}

ASTLiteralNode::ASTLiteralNode(const ASTLiteralNode& orig) {
}

ASTLiteralNode::~ASTLiteralNode() {
}

void ASTLiteralNode::printNode(int indent) {
	ostringstream oss;
	
	printIndented("literal:", indent);
	printIndented("type: " + Scanner::namesRev[type], indent);
	oss << "value: " << value;
	printIndented(oss.str(), indent);
	
	if(next != NULL) {
		next->printNode(indent);
	}
}