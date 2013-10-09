/* 
 * File:   ASTLoopNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 2:28 AM
 */

#include "ASTLoopNode.h"

ASTLoopNode::ASTLoopNode() : ASTStatementNode(), statement(NULL) {
}

ASTLoopNode::ASTLoopNode(const ASTLoopNode& orig) : ASTStatementNode(orig), statement(orig.statement)  {
}

ASTLoopNode& ASTLoopNode::operator= (const ASTLoopNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
        statement = rhs.statement;
    
 
    // return the existing object
    return *this;
}

ASTLoopNode::~ASTLoopNode() {
    delete statement;
}

void ASTLoopNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	printIndented("loop", indent);
	printIndented("statement:", indent + 2);
	statement->printNode(indent + 4, output);
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}