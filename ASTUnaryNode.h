/* 
 * File:   ASTUnaryNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:47 PM
 */

#ifndef ASTUNARYNODE_H
#define	ASTUNARYNODE_H

#include "ASTExpressionNode.h"

class ASTUnaryNode : public ASTExpressionNode{
public:
    ASTUnaryNode();
    ASTUnaryNode(const ASTUnaryNode& orig);
    ASTUnaryNode& operator= (const ASTUnaryNode &rhs);
    virtual ~ASTUnaryNode();
	
	void printNode(int indent);
	
	int operation;
	int type;
	ASTExpressionNode * operand;
private:

};

#endif	/* ASTUNARYNODE_H */

