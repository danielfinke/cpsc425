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

class ASTVariableDeclarationNode : public ASTDeclarationNode{
public:
    ASTVariableDeclarationNode();
    ASTVariableDeclarationNode(const ASTVariableDeclarationNode& orig);
    ASTVariableDeclarationNode& operator= (const ASTVariableDeclarationNode &rhs);
    virtual ~ASTVariableDeclarationNode();
	
	virtual void printNode(int indent);
	
	bool isArray;
	ASTExpressionNode * arrayExp;
private:

};

#endif	/* ASTVARIABLEDECLARATIONNODE_H */

