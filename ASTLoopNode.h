/* 
 * File:   ASTLoopNode.h
 * Author: daniel
 *
 * Created on October 8, 2013, 2:28 AM
 */

#ifndef ASTLOOPNODE_H
#define	ASTLOOPNODE_H

#include "ASTStatementNode.h"

class ASTLoopNode : public ASTStatementNode {
public:
	ASTLoopNode();
	ASTLoopNode(const ASTLoopNode& orig);
        ASTLoopNode& operator= (const ASTLoopNode &rhs);
	virtual ~ASTLoopNode();
	
	void printNode(int indent);
	
	ASTStatementNode * statement;
private:

};

#endif	/* ASTLOOPNODE_H */

