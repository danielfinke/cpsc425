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
#include "ASTFunctionNode.h"

/*ASTReturnNode represents return statements within the code. It is a subclass of 
 * ASTStatementNode and overrides the print statement. It contains a pointer to 
 * the option expression.
 */
class ASTReturnNode : public ASTStatementNode{
public:
    ASTReturnNode();
    ASTReturnNode(const ASTReturnNode& orig);
    ASTReturnNode& operator = (const ASTReturnNode &rhs);
    virtual ~ASTReturnNode();
	
	void semAnalyze();
	void scopeAnalyze();
	void printNode(int indent, ostream * output);
	string genQuadruples();
    
    ASTExpressionNode * expression;
	ASTFunctionNode * funcScope;
private:
	bool returnAnalyze();
	void returnAnalyzeType();
};

#endif	/* ASTRETURNNODE_H */

