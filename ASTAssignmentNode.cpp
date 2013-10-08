/* 
 * File:   ASTAssignmentNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 1:19 AM
 */

#include "ASTAssignmentNode.h"

ASTAssignmentNode::ASTAssignmentNode() : ASTStatementNode(), isArray(false),
		idName(""), exp(NULL), arrayExp(NULL)
{
}

ASTAssignmentNode::ASTAssignmentNode(const ASTAssignmentNode& orig) : ASTStatementNode(orig),
		isArray(orig.isArray), idName(orig.idName), exp(orig.exp), arrayExp(orig.arrayExp)
{
}

ASTAssignmentNode& ASTAssignmentNode::operator= (const ASTAssignmentNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
    isArray = rhs.isArray;
	idName = rhs.idName;
	exp = rhs.exp;
	arrayExp = rhs.arrayExp;
 
    // return the existing object
    return *this;
}

ASTAssignmentNode::~ASTAssignmentNode() {
	delete exp;
	delete arrayExp;
}

void ASTAssignmentNode::printNode(int indent) {
	ostringstream oss;
	
	printIndented("assignment:", indent);
	printIndented("id: " + idName, indent);
	oss << "arrayIndex? " << (isArray ? "YES" : "NO");
	printIndented(oss.str(), indent);
	
	if(isArray) {
		printIndented("index:", indent);
		arrayExp->printNode(indent + 2);
	}
	
	printIndented("value:", indent);
	exp->printNode(indent + 2);
	
	if(next != NULL) {
		next->printNode(indent);
	}
}