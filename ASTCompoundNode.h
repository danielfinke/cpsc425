/* 
 * File:   ASTCompoundNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:42 PM
 */

#ifndef ASTCOMPOUNDNODE_H
#define	ASTCOMPOUNDNODE_H

#include "ASTNode.h"
#include "ASTDeclarationNode.h"
#include "ASTStatementNode.h"

class ASTCompoundNode : public ASTStatementNode{
public:
    ASTCompoundNode();
    ASTCompoundNode(const ASTCompoundNode& orig);
    ASTCompoundNode& operator= (const ASTCompoundNode &rhs);
    virtual ~ASTCompoundNode();
	
	virtual void printNode(int indent, ostream * output);
	
	ASTDeclarationNode * dec;
	ASTStatementNode * statement;
private:

};

#endif	/* ASTCOMPOUNDNODE_H */

