/* 
 * File:   ASTParamNode.h
 * Author: daniel
 *
 * Created on October 7, 2013, 11:15 PM
 */

#ifndef ASTPARAMNODE_H
#define	ASTPARAMNODE_H

#include "ASTVariableDeclarationNode.h"

/*ASTParamNode represents parameters in function declarations. It is a subclass
 * of ASTVariableDeclaration and overrides the print method. it is kept as a list
 * with the FunctionNode pointing to the first param in the list.
 * It keeps track of if the parameter is a reference or not.
 */
class ASTParamNode : public ASTVariableDeclarationNode {
public:
	ASTParamNode();
	ASTParamNode(const ASTParamNode& orig);
        ASTParamNode& operator= (const ASTParamNode &rhs);
	virtual ~ASTParamNode();
	
	void printNode(int indent, ostream * output);
	
	bool isRef;
private:

};

#endif	/* ASTPARAMNODE_H */

