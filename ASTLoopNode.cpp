/* 
 * File:   ASTLoopNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 2:28 AM
 */

#include "ASTLoopNode.h"

ASTLoopNode::ASTLoopNode() : ASTStatementNode(), statement(NULL) {
}

ASTLoopNode::ASTLoopNode(const ASTLoopNode& orig) {
}

ASTLoopNode::~ASTLoopNode() {
}

void ASTLoopNode::printNode(int indent) {
	printIndented("loop:", indent);
	printIndented("statement:", indent);
	statement->printNode(indent + 2);
	
	if(next != NULL) {
		next->printNode(indent);
	}
}