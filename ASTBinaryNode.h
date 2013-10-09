/* 
 * File:   ASTBinaryNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:47 PM
 */

#ifndef ASTBINARYNODE_H
#define	ASTBINARYNODE_H

#include "ASTExpressionNode.h"

class ASTBinaryNode : public ASTExpressionNode {
public:
    ASTBinaryNode();
    ASTBinaryNode(const ASTBinaryNode& orig);
    ASTBinaryNode& operator= (const ASTBinaryNode &rhs);
    virtual ~ASTBinaryNode();
    
	void printNode(int indent);
	
	int oper;
	ASTExpressionNode * left, *right;
private:

};

#endif	/* ASTBINARYNODE_H */

