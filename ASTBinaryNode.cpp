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


void ASTBinaryNode ::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    this->left->semAnalyze();
    this->right->semAnalyze();
    this->typeAnalyze();

    if(this->next != NULL)
        this->next->semAnalyze();
    
}

void ASTBinaryNode::scopeAnalyze(){
    
    //nothing to do?
}

void ASTBinaryNode::typeAnalyze() {
	if(left == NULL || right == NULL) {
		// Throw exception
	}
	
	if(left->type == -1 || right->type == -1) {
		type = -1;
		return;
	}
	
	if(isArithmeticOper()) {
		if(left->type != Scanner::INT ||
				right->type != Scanner::INT) {
			// Semantic error - incorrect types for operator
			type = -1;
		}
		else {
			type = Scanner::INT;
		}
	}
	else if(isLogicOper()) {
		if(left->type != Scanner::BOOL ||
				right->type != Scanner::BOOL) {
			// Semantic error - incorrect types for operator
			type = -1;
		}
		else {
			type = Scanner::BOOL;
		}
	}
	else if(isRelationalOper()) {
		if(left->type != right->type) {
			// Semantic error - mismatched types
			type = -1;
		}
		else {
			type = Scanner::BOOL;
		}
	}
}

bool ASTBinaryNode::isArithmeticOper() {
	return oper == Scanner::PLUS ||
			oper == Scanner::MINUS ||
			oper == Scanner::MULT ||
			oper == Scanner::DIV ||
			oper == Scanner::MOD;
}

bool ASTBinaryNode::isLogicOper() {
	return oper == Scanner::AND ||
			oper == Scanner::ANDTHEN ||
			oper == Scanner::OR ||
			oper == Scanner::ORELSE;
}

bool ASTBinaryNode::isRelationalOper() {
	return oper == Scanner::LTEQ ||
			oper == Scanner::LT ||
			oper == Scanner::GT ||
			oper == Scanner::GTEQ ||
			oper == Scanner::EQ ||
			oper == Scanner::NEQ;
}

void ASTBinaryNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	printIndented("binop", indent);
	//printIndented("type: " + Scanner::namesRev[type], indent);
	printIndented("operation: " + Scanner::namesRev[oper], indent + 2);
	printIndented("left:", indent + 2);
	left->printNode(indent + 4, output);
	printIndented("right:", indent + 2);
	right->printNode(indent + 4, output);
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}