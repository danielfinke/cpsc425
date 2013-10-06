/* 
 * File:   ASTStatementNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:43 PM
 */

#ifndef ASTSTATEMENTNODE_H
#define	ASTSTATEMENTNODE_H

class ASTStatementNode : public ASTNode {
public:
    ASTStatementNode();
    ASTStatementNode(const ASTStatementNode& orig);
    virtual ~ASTStatementNode();
private:

};

#endif	/* ASTSTATEMENTNODE_H */

