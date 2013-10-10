/* 
 * File:   ASTVariableNode.h
 * Author: daniel
 *
 * Created on October 8, 2013, 8:51 AM
 */

#ifndef ASTVARIABLENODE_H
#define	ASTVARIABLENODE_H

#include "ASTFunctionCallNode.h"

/*ASTVariable Node represents variables in the language.
 * It is a subclass of ASTExpressionNode.and overrides the print method
 *  the node contains :
 *      -the id of the ID (identifier name)
 *      -if it is an array or not
 *      -a pointer to the expression denoting where in the array we are looking
 *              (e.g. x[4], 4 is the expression.)
 */
class ASTVariableNode : public ASTExpressionNode {
public:
	ASTVariableNode();
	ASTVariableNode(const ASTVariableNode& orig);
        ASTVariableNode& operator= (const ASTVariableNode &rhs);
	virtual ~ASTVariableNode();
	
	void printNode(int indent, ostream * output);
	
	int id;
	bool isArray;
	ASTExpressionNode * arrayExp;
private:

};

#endif	/* ASTVARIABLENODE_H */

