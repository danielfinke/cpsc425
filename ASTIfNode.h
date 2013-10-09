/* 
 * File:   ASTIfNode.h
 * Author: daniel
 *
 * Created on October 8, 2013, 1:35 AM
 */

#ifndef ASTIFNODE_H
#define	ASTIFNODE_H

#include "ASTExpressionNode.h"
#include "ASTStatementNode.h"

class ASTIfNode : public ASTStatementNode {
public:
	ASTIfNode();
	ASTIfNode(const ASTIfNode& orig);
        ASTIfNode& operator= (const ASTIfNode &rhs);
	virtual ~ASTIfNode();
	
	void printNode(int indent);
	
	ASTExpressionNode * exp;
	ASTStatementNode * statement, *elseStatement;
private:

};

#endif	/* ASTIFNODE_H */

