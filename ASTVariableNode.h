/* 
 * File:   ASTVariableNode.h
 * Author: daniel
 *
 * Created on October 8, 2013, 8:51 AM
 */

#ifndef ASTVARIABLENODE_H
#define	ASTVARIABLENODE_H

#include "ASTFunctionCallNode.h"

class ASTVariableNode : public ASTExpressionNode {
public:
	ASTVariableNode();
	ASTVariableNode(const ASTVariableNode& orig);
        ASTVariableNode& operator= (const ASTVariableNode &rhs);
	virtual ~ASTVariableNode();
	
	void printNode(int indent, ostream * output);
	
	string idName;
	bool isArray;
	ASTExpressionNode * arrayExp;
private:

};

#endif	/* ASTVARIABLENODE_H */

