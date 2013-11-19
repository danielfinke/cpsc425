/* 
 * File:   ASTBinaryNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:47 PM
 */

#include "ASTBinaryNode.h"
#include "ASTLiteralNode.h"
#include "SemanticAnalyzer.h"

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

void ASTBinaryNode::semAnalyze(bool restrictIdents){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
    this->left->semAnalyze(restrictIdents);
    this->right->semAnalyze(restrictIdents);
    this->typeAnalyze();

    if(this->next != NULL)
        this->next->semAnalyze();
}

void ASTBinaryNode::scopeAnalyze(){
    
    //nothing to do?
}

void ASTBinaryNode::typeAnalyze() {
	if(left == NULL || right == NULL) {
		type = -1;
		return;
	}
	
	if(left->type == -1 || right->type == -1) {
		type = -1;
		return;
	}
	
	if(isArithmeticOper()) {
		if((left->type != Scanner::INT &&
				left->type != Scanner::NUM)
				||
				(right->type != Scanner::INT &&
				right->type != Scanner::NUM)) {
			// Semantic error - incorrect types for operator
			sa->semanticError("Incorrect types for operator: " +
					Scanner::namesRev[oper], lineNumber);
			type = -1;
		}
		else {
			type = Scanner::INT;
		}
	}
	else if(isLogicOper()) {
		if((left->type != Scanner::BOOL &&
				left->type != Scanner::BLIT)
				||
				(right->type != Scanner::BOOL &&
				right->type != Scanner::BLIT)) {
			// Semantic error - incorrect types for operator
			sa->semanticError("Incorrect types for operator: " +
					Scanner::namesRev[oper], lineNumber);
			type = -1;
		}
		else {
			type = Scanner::BOOL;
		}
	}
	else if(isRelationalOper()) {
		if((left->type == Scanner::INT ||
				left->type == Scanner::NUM) &&
				(right->type == Scanner::INT || 
				right->type == Scanner::NUM)) {
			// OK
			type = Scanner::BOOL;
		}
		else if((left->type == Scanner::BOOL ||
				left->type == Scanner::BLIT) &&
				(right->type == Scanner::BOOL || 
				right->type == Scanner::BLIT)) {
			// OK
			type = Scanner::BOOL;
		}
		else {
			// Semantic error - mismatched types
			sa->semanticError("Mismatched types for operator: " +
					Scanner::namesRev[oper] +
					" Found " + Scanner::namesRev[left->type] +
					" and " + Scanner::namesRev[right->type], lineNumber);
			type = -1;
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

ASTLiteralNode * ASTBinaryNode::calc() {
	ASTLiteralNode * ret = new ASTLiteralNode, * leftVal, * rightVal;
	ret->type = type;
	
	leftVal = left->calc();
	if(leftVal == NULL) {
		return NULL;
	}
	
	// Short circuit exceptions
	if(oper == Scanner::ANDTHEN) {
		if(!leftVal) {
			ret->value = leftVal->value;
			return ret;
		}
	}
	else if(oper == Scanner::ORELSE) {
		if(leftVal) {
			ret->value = leftVal->value;
			return ret;
		}
	}
	
	rightVal = right->calc();
	if(rightVal == NULL) {
		return NULL;
	}
	
	switch(oper) {
		case Scanner::PLUS:
			ret->value = leftVal->value + rightVal->value;
			break;
		case Scanner::MINUS:
			ret->value = leftVal->value - rightVal->value;
			break;
		case Scanner::MULT:
			ret->value = leftVal->value * rightVal->value;
			break;
		case Scanner::DIV:
			ret->value = leftVal->value / rightVal->value;
			break;
		case Scanner::MOD:
			ret->value = leftVal->value % rightVal->value;
			break;
		case Scanner::AND:
			ret->value = leftVal->value && rightVal->value;
			break;
		case Scanner::OR:
			ret->value = leftVal->value || rightVal->value;
			break;
		case Scanner::LTEQ:
			ret->value = leftVal->value <= rightVal->value;
			break;
		case Scanner::LT:
			ret->value = leftVal->value < rightVal->value;
			break;
		case Scanner::GT:
			ret->value = leftVal->value > rightVal->value;
			break;
		case Scanner::GTEQ:
			ret->value = leftVal->value >= rightVal->value;
			break;
		case Scanner::EQ:
			ret->value = leftVal->value == rightVal->value;
			break;
		case Scanner::NEQ:
			ret->value = leftVal->value != rightVal->value;
			break;
	}
	
	return ret;
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