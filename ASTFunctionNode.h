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

/*ASTfunctionNode is a subclass of ASTDeclarationNode and represents function 
 * declarations in the language. It keeps track of :
 *              -a list of parameters required by the function
 *              -a list of compound statements within the function
 */
class ASTFunctionNode : public ASTDeclarationNode{
public:
    ASTFunctionNode();
    ASTFunctionNode(const ASTFunctionNode& orig);
    ASTFunctionNode& operator= (const ASTFunctionNode &rhs);
    virtual ~ASTFunctionNode();
	
        void semAnalyze();
        void scopeAnalyze();
	void printNode(int indent, ostream * output);
	
	ASTParamNode * param;
	ASTCompoundNode * compound;
private:

};

#endif	/* ASTFUNCTIONNODE_H */

