/* 
 * File:   ASTFunctionCallNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:40 PM
 */

#include "ASTFunctionCallNode.h"

ASTFunctionCallNode::ASTFunctionCallNode() : ASTStatementNode(), ASTExpressionNode(), idName(""), argument(NULL) {
}

ASTFunctionCallNode::ASTFunctionCallNode(const ASTFunctionCallNode& orig): ASTStatementNode(orig), 
        ASTExpressionNode(orig), idName(orig.idName),argument(orig.argument)
{
}

ASTFunctionCallNode& ASTFunctionCallNode::operator= (const ASTFunctionCallNode &rhs)
{
	ASTStatementNode::operator=(rhs);
        ASTExpressionNode::operator = (rhs);
	
    // do the copy
        idName= rhs.idName;
        argument = rhs.argument;
 
    // return the existing object
    return *this;
}

ASTFunctionCallNode::~ASTFunctionCallNode() {
    delete argument;
}

void ASTFunctionCallNode::printNode(int indent) {
	ASTStatementNode::printIndented("function call", indent);
	ASTStatementNode::printIndented("id: " + idName, indent + 2);
	
	if(argument != NULL) {
		ASTStatementNode::printIndented("args:", indent + 2);
		argument->printNode(indent + 4);
	}
	
	if(ASTStatementNode::next != NULL) {
		ASTStatementNode::next->printNode(indent);
	}
}