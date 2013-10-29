/* 
 * File:   ASTUnaryNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:47 PM
 */

#include "ASTUnaryNode.h"

ASTUnaryNode::ASTUnaryNode() : ASTExpressionNode(), operation(0), type(0), operand(NULL) {
}

ASTUnaryNode::ASTUnaryNode(const ASTUnaryNode& orig) :ASTExpressionNode(orig),
        operation(orig.operation), type(orig.type), operand(orig.operand)
{
}

ASTUnaryNode& ASTUnaryNode::operator= (const ASTUnaryNode &rhs)
{
	ASTExpressionNode::operator=(rhs);
	
    // do the copy
        operation=rhs.operation;
        type =rhs.type;
        operand = rhs.operand;
    
    // return the existing object
    return *this;
}

ASTUnaryNode::~ASTUnaryNode() {
    delete operand;
}

void ASTUnaryNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
     if(this->next != NULL)
        this->next->semAnalyze();
    
    this->typeAnalyze();
    
}

void ASTUnaryNode::scopeAnalyze(){
    
    
}

void ASTUnaryNode::typeAnalyze() {
	if(operand == NULL) {
		// Throw exception
	}
	
	if(operand->type == -1) {
		type = -1;
		return;
	}
	
	if(operation == Scanner::MINUS) {
		if(operand->type != Scanner::INT) {
			// Semantic error - incorrect types for operator
			type = -1;
		}
		else {
			type = Scanner::INT;
		}
	}
	else if(operation == Scanner::NOT) {
		if(operand->type != Scanner::BOOL) {
			// Semantic error - incorrect types for operator
			type = -1;
		}
		else {
			type = Scanner::BOOL;
		}
	}
}

void ASTUnaryNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	printIndented("unary exp", indent);
	//printIndented("type: " + Scanner::namesRev[type], indent + 2);
	printIndented("operation: " + Scanner::namesRev[operation], indent + 2);
	printIndented("operand:", indent + 2);
	operand->printNode(indent + 4, output);
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}