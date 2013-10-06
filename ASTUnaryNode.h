/* 
 * File:   ASTUnaryNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:47 PM
 */

#ifndef ASTUNARYNODE_H
#define	ASTUNARYNODE_H

class ASTUnaryNode : public ASTExpressionNode{
public:
    ASTUnaryNode();
    ASTUnaryNode(const ASTUnaryNode& orig);
    virtual ~ASTUnaryNode();
private:

};

#endif	/* ASTUNARYNODE_H */

