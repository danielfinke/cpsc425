/* 
 * File:   ASTStatementNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:43 PM
 */

#ifndef ASTSTATEMENTNODE_H
#define	ASTSTATEMENTNODE_H

#include "ASTNode.h"

/*ASTStatementNode inherits from ASTNode. As c++ does not use interface, 
 *  it is a super class to all statements,acting as an empty interface.
 */
class ASTStatementNode : public virtual ASTNode {
public:
    ASTStatementNode();
    ASTStatementNode(const ASTStatementNode& orig);
    ASTStatementNode& operator= (const ASTStatementNode &rhs);
    virtual ~ASTStatementNode();
	
	virtual void printNode(int indent, ostream * output);
private:

};

#endif	/* ASTSTATEMENTNODE_H */

