/* 
 * File:   ASTVariableNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:46 PM
 */

#ifndef ASTVARIABLENODE_H
#define	ASTVARIABLENODE_H

class ASTVariableNode : public ASTDeclarationNode{
public:
    ASTVariableNode();
    ASTVariableNode(const ASTVariableNode& orig);
    virtual ~ASTVariableNode();
private:

};

#endif	/* ASTVARIABLENODE_H */

