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
#include "ASTFunctionNode.h"

using namespace std;

/*ASTFunctionCallNode represents a function call in the code. It is a subclass
 * of both ASTExpressionNode and ASTStatementNode. This is because setting a
 * variable to the returning value from a function call (e.g. x=foo()) is 
 * considered an expression, where as a function call on its own (e.g. foo()) is
 * a statement. It overrides the print function
 * The ASTFunctionCallNode keeps track of:
 *              -the id of the function (the function's name)
 *              - the arguments as a list
 */

class ASTFunctionCallNode : public ASTStatementNode, ASTExpressionNode {
public:
    ASTFunctionCallNode();
    ASTFunctionCallNode(const ASTFunctionCallNode& orig);
    ASTFunctionCallNode& operator= (const ASTFunctionCallNode &rhs);
    virtual ~ASTFunctionCallNode();
	
	void semAnalyze();
	void semAnalyze(bool restrictIdents);
	void scopeAnalyze();
	void typeAnalyze();
	void printNode(int indent, ostream * output);
        void genQuadruples();
	
	int id;
	ASTFunctionNode * funcDec;
	ASTExpressionNode * argument;
        bool isStatement;
private:
	int getArgCount();
};

#endif	/* ASTFUNCTIONCALLNODE_H */

