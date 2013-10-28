/* 
 * File:   ASTExpressionNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:42 PM
 */

#ifndef ASTEXPRESSIONNODE_H
#define	ASTEXPRESSIONNODE_H

#include "ASTNode.h"

/*ASTExpressionNode represents general expressions within the language. It is a
 * subclass of ASTNode and an superclass to ASTBinaryNode, ASTUnaryNode ASTLiteralNode,
 * ASTVariableNode and ASTFunctionCallNode. It overrides the print method.
 * The "type" is a place holder for the type the expression returns
 * (e.g. int+int is of type int). this will be used later during semantic 
 * analysis.
 */

class ASTExpressionNode :public virtual ASTNode {
public:
    ASTExpressionNode();
    ASTExpressionNode(const ASTExpressionNode& orig);
    ASTExpressionNode& operator= (const ASTExpressionNode &rhs);
    virtual ~ASTExpressionNode();
	
        virtual void semAnalyze() = 0;
        virtual void scopeAnalyze() = 0;
	virtual void printNode(int indent, ostream * output) = 0;
	
	int type;
private:

};

#endif	/* ASTEXPRESSIONNODE_H */

