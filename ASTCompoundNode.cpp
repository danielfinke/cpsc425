/* 
 * File:   ASTCompoundNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:42 PM
 */

#include "ASTCompoundNode.h"

ASTCompoundNode::ASTCompoundNode() : ASTNode(), dec(0), statement(0) {
}

ASTCompoundNode::ASTCompoundNode(const ASTCompoundNode& orig):ASTNode(orig),
		dec(orig.dec), statement(orig.statement)
{
}

ASTCompoundNode& ASTCompoundNode::operator= (const ASTCompoundNode &rhs)
{
	ASTNode::operator=(rhs);
	
	dec = rhs.dec;
	statement = rhs.statement;
 
    // return the existing object
    return *this;
}

ASTCompoundNode::~ASTCompoundNode() {
	delete dec;
	delete statement;
}

void ASTCompoundNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	if(dec != NULL) {
		printIndented("declarations", indent);
		dec->printNode(indent + 2, output);
	}
	
	if(statement != NULL) {
		printIndented("statements", indent);
		statement->printNode(indent + 2, output);
	}
	
	if(next != NULL) {
		next->printNode(indent, output);
	}	
}