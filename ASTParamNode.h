/* 
 * File:   ASTParamNode.h
 * Author: daniel
 *
 * Created on October 7, 2013, 11:15 PM
 */

#ifndef ASTPARAMNODE_H
#define	ASTPARAMNODE_H

#include "ASTVariableDeclarationNode.h"

class ASTParamNode : public ASTVariableDeclarationNode {
public:
	ASTParamNode();
	ASTParamNode(const ASTParamNode& orig);
        ASTParamNode& operator= (const ASTParamNode &rhs);
	virtual ~ASTParamNode();
	
	void printNode(int indent);
	
	bool isRef;
private:

};

#endif	/* ASTPARAMNODE_H */

