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
/*The ASTCompoundNode represents compound statements within the code. It is a 
 * subclass of the ASTStatementNode and contains an override of the
 * print statement. It has a an ASTDeclarationNodeChild representing the first 
 * declaration (if any), and an ASTStatementNode child representing the first
 * statement in the list of statements.
 */

class ASTCompoundNode : public virtual ASTStatementNode{
public:
    ASTCompoundNode();
    ASTCompoundNode(const ASTCompoundNode& orig);
    ASTCompoundNode& operator= (const ASTCompoundNode &rhs);
    virtual ~ASTCompoundNode();
	
	virtual void semAnalyze();
	virtual void scopeAnalyze();
	virtual void printNode(int indent, ostream * output);
        virtual void genQuadruples();
	
	bool returnAnalyze();
	
	ASTDeclarationNode * dec;
	ASTStatementNode * statement;
private:

};

#endif	/* ASTCOMPOUNDNODE_H */

