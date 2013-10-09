/* 
 * File:   ASTVariableNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 8:51 AM
 */

#include "ASTVariableNode.h"

ASTVariableNode::ASTVariableNode() : ASTExpressionNode(), idName(""), isArray(false),
		arrayExp(NULL)
{
}

ASTVariableNode::ASTVariableNode(const ASTVariableNode& orig) : ASTExpressionNode(orig),
        idName(orig.idName), isArray(orig.isArray)
{
}

ASTVariableNode& ASTVariableNode::operator= (const ASTVariableNode &rhs)
{
	ASTExpressionNode::operator=(rhs);
	
    // do the copy
        idName = rhs.idName;
        isArray = rhs.isArray;
 
    // return the existing object
    return *this;
}

ASTVariableNode::~ASTVariableNode() {
}

void ASTVariableNode::printNode(int indent, ostream * output) {
	ostringstream oss;
	
	this->output = output;
	
	printIndented("var", indent);
	printIndented("name: " + idName, indent + 2);
	printIndented("type: " + Scanner::namesRev[type], indent + 2);
	oss << "array? " << (isArray ? "YES" : "NO");
	printIndented(oss.str(), indent + 2);
	
	if(isArray) {
		printIndented("index:", indent + 2);
		arrayExp->printNode(indent + 4, output);
	}
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}