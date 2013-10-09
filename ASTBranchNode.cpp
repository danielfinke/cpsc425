/* 
 * File:   ASTBranchNode.cpp
 * Author: claire
 * 
 * Created on October 9, 2013, 12:09 AM
 */

#include "ASTBranchNode.h"

ASTBranchNode::ASTBranchNode(): ASTStatementNode(), expression(NULL), firstCase(NULL){
}

ASTBranchNode::ASTBranchNode(const ASTBranchNode& orig):ASTStatementNode(orig),
        expression(orig.expression), firstCase(orig.firstCase)
{
}

ASTBranchNode& ASTBranchNode::operator= (const ASTBranchNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
        expression = rhs.expression;
        firstCase = rhs.firstCase;
    
 
    // return the existing object
        return *this;
}
  

ASTBranchNode::~ASTBranchNode() {
    delete expression;
    delete firstCase;
}

