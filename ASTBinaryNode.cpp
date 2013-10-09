/* 
 * File:   ASTBinaryNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:47 PM
 */

#include "ASTBinaryNode.h"

ASTBinaryNode::ASTBinaryNode() : ASTExpressionNode(), oper(0), left(NULL), right(NULL) {
}

ASTBinaryNode::ASTBinaryNode(const ASTBinaryNode& orig):ASTExpressionNode(orig),oper(orig.oper),
left(orig.left), right(orig.right)
{
}

ASTBinaryNode& ASTBinaryNode::operator= (const ASTBinaryNode &rhs)
{
	ASTExpressionNode::operator=(rhs);
	
    // do the copy
        oper = rhs.oper;
        left = rhs.left;
        right = rhs.right;
 
    // return the existing object
    return *this;
}

ASTBinaryNode::~ASTBinaryNode() {
    delete left;
    delete right;
}

void ASTBinaryNode::printNode(int indent) {
	printIndented("binop", indent);
	//printIndented("type: " + Scanner::namesRev[type], indent);
	printIndented("operation: " + Scanner::namesRev[oper], indent + 2);
	printIndented("left:", indent + 2);
	left->printNode(indent + 4);
	printIndented("right:", indent + 2);
	right->printNode(indent + 4);
	
	if(next != NULL) {
		next->printNode(indent);
	}
}