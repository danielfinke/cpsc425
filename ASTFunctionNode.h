/* 
 * File:   ASTFunctionNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:47 PM
 */

#ifndef ASTFUNCTIONNODE_H
#define	ASTFUNCTIONNODE_H

class ASTFunctionNode : public ASTDeclarationNode{
public:
    ASTFunctionNode();
    ASTFunctionNode(const ASTFunctionNode& orig);
    virtual ~ASTFunctionNode();
private:

};

#endif	/* ASTFUNCTIONNODE_H */

