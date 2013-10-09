/* 
 * File:   ASTStatementNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:43 PM
 */

#ifndef ASTSTATEMENTNODE_H
#define	ASTSTATEMENTNODE_H

#include "ASTNode.h"

class ASTStatementNode : public ASTNode {
public:
    ASTStatementNode();
    ASTStatementNode(const ASTStatementNode& orig);
    ASTStatementNode& operator= (const ASTStatementNode &rhs);
    virtual ~ASTStatementNode();
	
	virtual void printNode(int indent, ostream * output);
private:

};

#endif	/* ASTSTATEMENTNODE_H */

