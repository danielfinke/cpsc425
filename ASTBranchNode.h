/* 
 * File:   ASTBranchNode.h
 * Author: claire
 *
 * Created on October 9, 2013, 12:09 AM
 */

#ifndef ASTBRANCHNODE_H
#define	ASTBRANCHNODE_H

#include "ASTStatementNode.h"
#include "ASTExpressionNode.h"
#include "ASTCaseNode.h"

class ASTBranchNode : public ASTStatementNode{
public:
    ASTBranchNode();
    ASTBranchNode(const ASTBranchNode& orig);
    ASTBranchNode& operator = (const ASTBranchNode &rhs);
    virtual ~ASTBranchNode();
    
    ASTExpressionNode * expression;
    ASTCaseNode * firstCase;
    
private:

};

#endif	/* ASTBRANCHNODE_H */

