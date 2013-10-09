/* 
 * File:   ASTCaseNode.cpp
 * Author: claire
 * 
 * Created on October 9, 2013, 12:09 AM
 */

#include "ASTCaseNode.h"

ASTCaseNode::ASTCaseNode() : ASTStatementNode(),statement(NULL), nextCase(NULL), type(0), num(0) {
}

ASTCaseNode::ASTCaseNode(const ASTCaseNode& orig):ASTStatementNode(orig), statement(orig.statement),
        nextCase(orig.nextCase), type(orig.type), num(orig.num)
{
}

ASTCaseNode& ASTCaseNode::operator= (const ASTCaseNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
        statement = rhs.statement;
        nextCase = rhs.nextCase;
        type = rhs.type;
        num = rhs.num;
    
    // return the existing object
        return *this;
}

ASTCaseNode::~ASTCaseNode() {
    delete statement;
    delete nextCase;
}

