/* 
 * File:   ASTIfNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 1:35 AM
 */

#include "ASTIfNode.h"

ASTIfNode::ASTIfNode() : ASTStatementNode(), exp(NULL), statement(NULL), elseStatement(NULL) {
}

ASTIfNode::ASTIfNode(const ASTIfNode& orig) {
}

ASTIfNode::~ASTIfNode() {
}

void ASTIfNode::printNode(int indent) {
	printIndented("if:", indent);
	printIndented("condition:", indent);
	exp->printNode(indent + 2);
	printIndented("ifStatement:", indent);
	statement->printNode(indent + 2);
	
	if(elseStatement != NULL) {
		printIndented("elseStatement:", indent);
		elseStatement->printNode(indent + 2);
	}
	
	if(next != NULL) {
		next->printNode(indent);
	}
}