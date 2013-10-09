/* 
 * File:   ASTCompoundNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:42 PM
 */

#ifndef ASTCOMPOUNDNODE_H
#define	ASTCOMPOUNDNODE_H

#include "ASTNode.h"

class ASTCompoundNode :public ASTNode{
public:
    ASTCompoundNode();
    ASTCompoundNode(const ASTCompoundNode& orig);
    virtual ~ASTCompoundNode();
	
	// todo: make pure virtual
	virtual void printNode(int indent);
private:

};

#endif	/* ASTCOMPOUNDNODE_H */

