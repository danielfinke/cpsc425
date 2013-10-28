/* 
 * File:   ASTExpressionNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:42 PM
 */

#include "ASTExpressionNode.h"

ASTExpressionNode::ASTExpressionNode() : ASTNode(), type(0) {
}

ASTExpressionNode::ASTExpressionNode(const ASTExpressionNode& orig):ASTNode(orig), type(orig.type) {
}

ASTExpressionNode& ASTExpressionNode::operator= (const ASTExpressionNode &rhs)
{
	ASTNode::operator=(rhs);
	
    // do the copy
        type = rhs.type;
 
    // return the existing object
    return *this;
}


ASTExpressionNode::~ASTExpressionNode() {
}
