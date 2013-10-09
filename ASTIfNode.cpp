/* 
 * File:   ASTIfNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 1:35 AM
 */

#include "ASTIfNode.h"

ASTIfNode::ASTIfNode() : ASTStatementNode(), exp(NULL), statement(NULL), elseStatement(NULL) {
}

ASTIfNode::ASTIfNode(const ASTIfNode& orig) : ASTStatementNode(orig), exp(orig.exp),
        statement(orig.statement),elseStatement(orig.statement)
{
}

ASTIfNode& ASTIfNode::operator= (const ASTIfNode &rhs)
{
	ASTStatementNode::operator=(rhs);
	
    // do the copy
        exp=rhs.exp;
        statement = rhs.statement;
        elseStatement = rhs.elseStatement;
    
 
    // return the existing object
    return *this;
}

ASTIfNode::~ASTIfNode() {
    delete exp;
    delete statement;
    delete elseStatement;
}

void ASTIfNode::printNode(int indent) {
	printIndented("if", indent);
	printIndented("condition:", indent + 2);
	exp->printNode(indent + 4);
	printIndented("true branch:", indent + 2);
	statement->printNode(indent + 4);
	
	if(elseStatement != NULL) {
		printIndented("false branch:", indent + 2);
		elseStatement->printNode(indent + 4);
	}
	
	if(next != NULL) {
		next->printNode(indent);
	}
}