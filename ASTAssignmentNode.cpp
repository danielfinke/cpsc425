/* 
 * File:   ASTAssignmentNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 1:19 AM
 */

#include "ASTAssignmentNode.h"

ASTAssignmentNode::ASTAssignmentNode() : ASTStatementNode(), isArray(false),
		id(0), exp(NULL), arrayExp(NULL)
{
}

ASTAssignmentNode::ASTAssignmentNode(const ASTAssignmentNode& orig) : ASTStatementNode(orig),
		isArray(orig.isArray), id(orig.id), exp(orig.exp), arrayExp(orig.arrayExp)
{
}

ASTAssignmentNode& ASTAssignmentNode::operator= (const ASTAssignmentNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
    isArray = rhs.isArray;
	id = rhs.id;
	exp = rhs.exp;
	arrayExp = rhs.arrayExp;
 
    // return the existing object
    return *this;
}

ASTAssignmentNode::~ASTAssignmentNode() {
	delete exp;
	delete arrayExp;
}

void ASTAssignmentNode::printNode(int indent, ostream * output) {
	ostringstream oss;
	
	this->output = output;
	
	printIndented("assignment", indent);
	if(ASTNode::lookup != NULL) {
		printIndented("id: " + ASTNode::lookup->getIdentifierName(id), indent + 2);
	}
	oss << "arrayIndex? " << (isArray ? "YES" : "NO");
	printIndented(oss.str(), indent + 2);
	
	if(isArray) {
		printIndented("index:", indent + 2);
		arrayExp->printNode(indent + 4, output);
	}
	
	printIndented("value:", indent + 2);
	exp->printNode(indent + 4, output);
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}