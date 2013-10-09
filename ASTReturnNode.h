/* 
 * File:   ASTReturnNode.h
 * Author: claire
 *
 * Created on October 9, 2013, 12:09 AM
 */

#ifndef ASTRETURNNODE_H
#define	ASTRETURNNODE_H

#include "ASTStatementNode.h"
#include "ASTExpressionNode.h"

class ASTReturnNode : public ASTStatementNode{
public:
    ASTReturnNode();
    ASTReturnNode(const ASTReturnNode& orig);
    ASTReturnNode& operator = (const ASTReturnNode &rhs);
    virtual ~ASTReturnNode();
    
    ASTExpressionNode * expression;
private:

};

#endif	/* ASTRETURNNODE_H */

