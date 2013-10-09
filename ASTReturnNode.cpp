/* 
 * File:   ASTReturnNode.cpp
 * Author: claire
 * 
 * Created on October 9, 2013, 12:09 AM
 */

#include "ASTReturnNode.h"

ASTReturnNode::ASTReturnNode(): ASTStatementNode(), expression(NULL) {
}

ASTReturnNode::ASTReturnNode(const ASTReturnNode& orig):ASTStatementNode(orig),expression(orig.expression) {
}

ASTReturnNode& ASTReturnNode::operator= (const ASTReturnNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
        expression = rhs.expression;

 
    // return the existing object
        return *this;
}

ASTReturnNode::~ASTReturnNode() {
    delete expression;
}

