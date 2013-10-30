/* 
 * File:   ASTVariableNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:46 PM
 */

#ifndef ASTVARIABLEDECLARATIONNODE_H
#define	ASTVARIABLEDECLARATIONNODE_H

#include <string>
#include "ASTDeclarationNode.h"
#include "ASTExpressionNode.h"

using namespace std;

/*ASTVariableDeclarationNode represents variable declarations. It is a subclass
 * of ASTDeclarationNode and a superclass to AStParamNode. It overrides the print
 * method. 
 * It contains the array information that may be associated with a delcartion.
 *      -if its an array or not
 *      -what the array expression is
 */
class ASTVariableDeclarationNode : public ASTDeclarationNode{
public:
    ASTVariableDeclarationNode();
    ASTVariableDeclarationNode(const ASTVariableDeclarationNode& orig);
    ASTVariableDeclarationNode& operator= (const ASTVariableDeclarationNode &rhs);
    virtual ~ASTVariableDeclarationNode();
	
	virtual void semAnalyze();
	virtual void scopeAnalyze();
	virtual void printNode(int indent, ostream * output);
	
	bool isArray;
	ASTExpressionNode * arrayExp;
private:

};

#endif	/* ASTVARIABLEDECLARATIONNODE_H */

