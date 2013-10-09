/* 
 * File:   ASTCaseNode.h
 * Author: claire
 *
 * Created on October 9, 2013, 12:09 AM
 */

#ifndef ASTCASENODE_H
#define	ASTCASENODE_H

#include "ASTStatementNode.h"

class ASTCaseNode : public ASTStatementNode {
public:
    ASTCaseNode();
    ASTCaseNode(const ASTCaseNode& orig);
    ASTCaseNode& operator = (const ASTCaseNode &rhs);
    virtual ~ASTCaseNode();
	
	void printNode(int indent, ostream * output);
    
    int type;
    int num;
    ASTStatementNode * statement;
    ASTCaseNode * nextCase;
private:

};

#endif	/* ASTCASENODE_H */

