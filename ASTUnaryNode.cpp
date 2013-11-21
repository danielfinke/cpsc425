/* 
 * File:   ASTUnaryNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:47 PM
 */

#include "ASTUnaryNode.h"
#include "ASTLiteralNode.h"
#include "SemanticAnalyzer.h"

ASTUnaryNode::ASTUnaryNode() : ASTExpressionNode(), operation(0), operand(NULL) {
}

ASTUnaryNode::ASTUnaryNode(const ASTUnaryNode& orig) :ASTExpressionNode(orig),
        operation(orig.operation), operand(orig.operand)
{
}

ASTUnaryNode& ASTUnaryNode::operator= (const ASTUnaryNode &rhs)
{
	ASTExpressionNode::operator=(rhs);
	
    // do the copy
        operation=rhs.operation;
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
	
	operand->semAnalyze();
	
    this->typeAnalyze();
	
     if(this->next != NULL)
        this->next->semAnalyze();
}

void ASTUnaryNode::semAnalyze(bool restrictIdents){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
	
	operand->semAnalyze(restrictIdents);
	
    this->typeAnalyze();
	
     if(this->next != NULL)
        this->next->semAnalyze();
}

void ASTUnaryNode::scopeAnalyze(){
    
    
}

void ASTUnaryNode::typeAnalyze() {
	if(operand == NULL) {
		type = -1;
		return;
	}
	
	if(operand->type == -1) {
		type = -1;
		return;
	}
	
	if(operation == Scanner::MINUS) {
		if(operand->type != Scanner::INT && operand->type != Scanner::NUM) {
			// Semantic error - incorrect types for operator
			sa->semanticError("Incorrect types for operator: " +
				Scanner::namesRev[Scanner::MINUS], lineNumber);
			type = -1;
		}
		else {
			type = Scanner::INT;
		}
	}
	else if(operation == Scanner::NOT) {
		if(operand->type != Scanner::BOOL && operand->type != Scanner::BLIT) {
			// Semantic error - incorrect types for operator
			sa->semanticError("Incorrect types for operator: " +
				Scanner::namesRev[Scanner::NOT], lineNumber);
			type = -1;
		}
		else {
			type = Scanner::BOOL;
		}
	}
}

ASTLiteralNode * ASTUnaryNode::calc() {
	ASTLiteralNode * ret = new ASTLiteralNode, * operandVal;
	ret->type = type;
	
	operandVal = operand->calc();
	if(operandVal == NULL) {
		return NULL;
	}
	
	switch(operation) {
		case Scanner::MINUS:
			ret->value = 0 - operandVal->value;
			break;
		case Scanner::NOT:
			ret->value = !operandVal->value;
			break;
	}
	
	return ret;
}

string ASTUnaryNode::genQuadruples(){
   //ceates the quadruple for unary functions
    Quadruple quad;
    quad.result=getTemp();
    quad.arg1 =operand->genQuadruples();
    switch(operation){
        case Scanner::MINUS:
            quad.operation="uminus";
            break;
        case Scanner::NOT:
            quad.operation="not";
            break;
    }
    
    vec.push_back(quad);
    return quad.result;
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