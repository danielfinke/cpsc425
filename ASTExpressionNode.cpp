/* 
 * File:   ASTExpressionNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:42 PM
 */

#include "ASTExpressionNode.h"

ASTExpressionNode::ASTExpressionNode() : ASTNode(), type(0), value(0) {
}

ASTExpressionNode::ASTExpressionNode(const ASTExpressionNode& orig):ASTNode(orig), type(orig.type),
		value(orig.value)
{
}

ASTExpressionNode& ASTExpressionNode::operator= (const ASTExpressionNode &rhs)
{
	ASTNode::operator=(rhs);
	
    // do the copy
    type = rhs.type;
	value = rhs.value;
 
    // return the existing object
    return *this;
}



ASTLiteralNode * ASTExpressionNode::calc() {
	return NULL;
}

ASTExpressionNode::~ASTExpressionNode() {
}
