/* 
 * File:   ASTFunctionNode.h
 * Author: claire
 *
 * Created on October 6, 2013, 2:47 PM
 */

#ifndef ASTFUNCTIONNODE_H
#define	ASTFUNCTIONNODE_H

#include "ASTDeclarationNode.h"
#include "ASTParamNode.h"
#include "ASTCompoundNode.h"

class ASTFunctionNode : public ASTDeclarationNode{
public:
    ASTFunctionNode();
    ASTFunctionNode(const ASTFunctionNode& orig);
    virtual ~ASTFunctionNode();
	
	void printNode(int indent);
	
	ASTParamNode * param;
	ASTCompoundNode * compound;
private:

};

#endif	/* ASTFUNCTIONNODE_H */

