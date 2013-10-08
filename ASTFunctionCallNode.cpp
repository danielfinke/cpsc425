/* 
 * File:   ASTFunctionCallNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:40 PM
 */

#include "ASTFunctionCallNode.h"

ASTFunctionCallNode::ASTFunctionCallNode() : ASTStatementNode(), idName(""), argument(NULL) {
}

ASTFunctionCallNode::ASTFunctionCallNode(const ASTFunctionCallNode& orig) {
}

ASTFunctionCallNode::~ASTFunctionCallNode() {
}

void ASTFunctionCallNode::printNode(int indent) {
	printIndented("function call:", indent);
	printIndented("name: " + idName, indent);
	
	if(argument != NULL) {
		printIndented("args:", indent);
		argument->printNode(indent + 2);
	}
	
	if(next != NULL) {
		next->printNode(indent);
	}
}