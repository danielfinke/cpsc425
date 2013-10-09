/* 
 * File:   ASTFunctionCallNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:40 PM
 */

#ifndef ASTFUNCTIONCALLNODE_H
#define	ASTFUNCTIONCALLNODE_H

#include <string>
#include "ASTNode.h"
#include "ASTExpressionNode.h"
#include "ASTStatementNode.h"

using namespace std;

class ASTFunctionCallNode : public ASTStatementNode, ASTExpressionNode {
public:
    ASTFunctionCallNode();
    ASTFunctionCallNode(const ASTFunctionCallNode& orig);
    virtual ~ASTFunctionCallNode();
	
	void printNode(int indent);
	
	string idName;
	ASTExpressionNode * argument;
private:

};

#endif	/* ASTFUNCTIONCALLNODE_H */

