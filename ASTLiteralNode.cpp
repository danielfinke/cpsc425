/* 
 * File:   ASTLiteralNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 9:08 AM
 */

#include "ASTLiteralNode.h"

ASTLiteralNode::ASTLiteralNode() : ASTExpressionNode(), value(0) {
}

ASTLiteralNode::ASTLiteralNode(const ASTLiteralNode& orig) : ASTExpressionNode(orig), value(orig.value) {
}

ASTLiteralNode& ASTLiteralNode::operator= (const ASTLiteralNode &rhs)
{
	ASTExpressionNode::operator=(rhs);
	
    // do the copy
        value = rhs.value;
 
    // return the existing object
    return *this;
}

ASTLiteralNode::~ASTLiteralNode() {
}

void ASTLiteralNode::printNode(int indent) {
	ostringstream oss;
	
	printIndented("literal", indent);
	printIndented("type: " + Scanner::namesRev[type], indent + 2);
	oss << "value: " << value;
	printIndented(oss.str(), indent + 2);
	
	if(next != NULL) {
		next->printNode(indent);
	}
}