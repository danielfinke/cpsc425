/* 
 * File:   ASTExpressionNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:42 PM
 */

#ifndef ASTEXPRESSIONNODE_H
#define	ASTEXPRESSIONNODE_H

#include "ASTNode.h"

class ASTExpressionNode :public ASTNode{
public:
    ASTExpressionNode();
    ASTExpressionNode(const ASTExpressionNode& orig);
    virtual ~ASTExpressionNode();
	
	virtual void printNode(int indent) = 0;
	
	int type;
private:

};

#endif	/* ASTEXPRESSIONNODE_H */

