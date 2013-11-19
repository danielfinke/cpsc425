/* 
 * File:   ASTBinaryNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:47 PM
 */

#include "ASTBinaryNode.h"
#include "ASTLiteralNode.h"
#include "SemanticAnalyzer.h"
#include "Quadruple.h"

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
    
    //nothing to do
}

string ASTBinaryNode::genQuadruples(){
    Quadruple quad = new Quadruple();
    quad.result = getTemp();
	
	if(oper == Scanner::ANDTHEN) {
		return genAndThenSS();
	}
	else if(oper == Scanner::ORELSE) {
		return genOrElseSS();
	}
	
    quad.arg1 = left->genQuadruples();
    quad.arg2 = right->genQuadruples();
    
    switch(oper){
        case Scanner::LTEQ:
            quad.operation="lte";
            break;
        case Scanner::LT:
            quad.operation="lt";
            break;
        case Scanner::GTEQ:
            quad.operation="gte";
            
            break;
        case Scanner::GT:
            quad.operation="gt";
            break;
        case Scanner::EQ:
            quad.operation="eq";
            break;
        case Scanner::NEQ:
            quad.operation="neq";
            break;
        case Scanner::PLUS:
            quad.operation="add";
            break;
        case Scanner::MINUS:
            quad.operation="sub";
            break;
        case Scanner::AND:
            quad.operation="and";
            break;
        case Scanner::OR:
            quad.operation="or";
            break;
        case Scanner::MULT:
            quad.operation="mult";
            break;
        case Scanner::DIV:
            quad.operation="div";
            break;
        case Scanner::MOD:
            quad.operation="mod";
            break;
    }
    
    vec.push_back(quad);
    return quad.result;
}

string ASTBinaryNode::genAndThenSS() {
	Quadruple quad;
	quad.operation = "iff";
	quad.arg1 = left->genQuadruples();
	quad.result = getLabel();
	
	Quadruple quad2;
	quad2.operation = "and";
	quad2.arg1 = quad.arg1;
	quad2.arg2 = right->genQuadruples();
	quad2.result = quad2.arg1;
	
	Quadruple quad3;
	quad3.operation = "lab";
	quad3.result = quad.result;
	
	vec.push_back(quad);
	vec.push_back(quad2);
	vec.push_back(quad3);
	
	return quad2.result;
}

string ASTBinaryNode::genOrElseSS() {
	Quadruple quad;
	quad.operation = "ift";
	quad.arg1 = left->genQuadruples();
	quad.result = getLabel();
	
	Quadruple quad2;
	quad2.operation = "or";
	quad2.arg1 = quad.arg1;
	quad2.arg2 = right->genQuadruples();
	quad2.result = quad2.arg1;
	
	Quadruple quad3;
	quad3.operation = "lab";
	quad3.result = quad.result;
	
	vec.push_back(quad);
	vec.push_back(quad2);
	vec.push_back(quad3);
	
	return quad2.result;
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