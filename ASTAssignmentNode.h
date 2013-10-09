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

using namespace std;

class ASTAssignmentNode : public ASTStatementNode {
public:
	ASTAssignmentNode();
	ASTAssignmentNode(const ASTAssignmentNode& orig);
	ASTAssignmentNode& operator= (const ASTAssignmentNode &rhs);
	virtual ~ASTAssignmentNode();
	
	void printNode(int indent, ostream * output);
	
	bool isArray;
	int id;
	ASTExpressionNode * exp, *arrayExp;
private:

};

#endif	/* ASTASSIGNMENTNODE_H */

