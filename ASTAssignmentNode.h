/* 
 * File:   ASTAssignmentNode.h
 * Author: daniel
 *
 * Created on October 8, 2013, 1:19 AM
 */

#ifndef ASTASSIGNMENTNODE_H
#define	ASTASSIGNMENTNODE_H

#include <string>
#include "ASTExpressionNode.h"
#include "ASTStatementNode.h"
#include "ASTDeclarationNode.h"

using namespace std;

/*The ASTAssignmentNode represents assignment operations within the program. It
 * is a subclass of the ASTStatementNode. It contains an override of the
 * print statement. This node keeps track of:
 *              -if it is an array or not
 *              -the id of what is being assigned to
 *              -pointer to the expression that is contained in the array portion
 *              -pointer to the operand expression
 * 
 */

class ASTAssignmentNode : public ASTStatementNode {
public:
	ASTAssignmentNode();
	ASTAssignmentNode(const ASTAssignmentNode& orig);
	ASTAssignmentNode& operator= (const ASTAssignmentNode &rhs);
	virtual ~ASTAssignmentNode();
	
	void semAnalyze();
	void scopeAnalyze();
	void typeAnalyze();
	void printNode(int indent, ostream * output);
        void genQuadruples();
	
	bool isArray;
	int id;
	ASTDeclarationNode * left;
	ASTExpressionNode * exp, *arrayExp;
private:

};

#endif	/* ASTASSIGNMENTNODE_H */

