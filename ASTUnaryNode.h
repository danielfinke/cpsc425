/* 
 * File:   ASTUnaryNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:47 PM
 */

#ifndef ASTUNARYNODE_H
#define	ASTUNARYNODE_H

#include "ASTExpressionNode.h"

/*ASTUnaryNode represents unary operations such as mod, and "-". It is a subclass
 * of ASTExpressionNode, and overrides the print method. It contains:
 *        -a type(int or bool) (eg. -4 is type int)
 *        -operation as an ENUM (not, minus,etc)
 *        - a pointer to the operand
 
 */
class ASTUnaryNode : public ASTExpressionNode{
public:
    ASTUnaryNode();
    ASTUnaryNode(const ASTUnaryNode& orig);
    ASTUnaryNode& operator= (const ASTUnaryNode &rhs);
    virtual ~ASTUnaryNode();
	
	void semAnalyze();
	void semAnalyze(bool restrictIdents);
	void scopeAnalyze();
	void typeAnalyze();
	
	ASTLiteralNode * calc();
	void printNode(int indent, ostream * output);
	
	int operation;
	int type;
	ASTExpressionNode * operand;
private:

};

#endif	/* ASTUNARYNODE_H */

