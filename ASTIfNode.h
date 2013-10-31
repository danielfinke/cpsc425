/* 
 * File:   ASTIfNode.h
 * Author: daniel
 *
 * Created on October 8, 2013, 1:35 AM
 */

#ifndef ASTIFNODE_H
#define	ASTIFNODE_H

#include "ASTExpressionNode.h"
#include "ASTStatementNode.h"

/*ASTIfNode represents if statement. It is a subclass of ASTStatementNode and 
 * overrides the SATNode print statement. It keeps track of:
 *              -the expression within the if statement
 *              -the statement within the if clause
 *              -the statement within the else clause
 */
class ASTIfNode : public ASTStatementNode {
public:
	ASTIfNode();
	ASTIfNode(const ASTIfNode& orig);
        ASTIfNode& operator= (const ASTIfNode &rhs);
	virtual ~ASTIfNode();
	
	void semAnalyze();
	void scopeAnalyze();
	void typeAnalyze();
	void printNode(int indent, ostream * output);
	
	bool returnAnalyze();
	
	ASTExpressionNode * exp;
	ASTStatementNode * statement, *elseStatement;
private:

};

#endif	/* ASTIFNODE_H */

