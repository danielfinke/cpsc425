/* 
 * File:   ASTLiteralNode.cpp
 * Author: daniel
 * 
 * Created on October 8, 2013, 9:08 AM
 */

#include "ASTLiteralNode.h"

ASTLiteralNode::ASTLiteralNode() : ASTExpressionNode() {
}

ASTLiteralNode::ASTLiteralNode(const ASTLiteralNode& orig) : ASTExpressionNode(orig) {
}

ASTLiteralNode& ASTLiteralNode::operator= (const ASTLiteralNode &rhs)
{
	ASTExpressionNode::operator=(rhs);
	
    // do the copy
 
    // return the existing object
    return *this;
}

ASTLiteralNode::~ASTLiteralNode() {
}

void ASTLiteralNode::semAnalyze(){
    
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
     if(this->next != NULL)
        this->next->semAnalyze();
    
    //this->typeAnalyze();
    
}

void ASTLiteralNode::semAnalyze(bool restrictIdents) {
	
    if(init || !this->isGlobalDec){
        this->scopeAnalyze();
        if(init)
            return;
    }
    
     if(this->next != NULL)
        this->next->semAnalyze();
    
    //this->typeAnalyze();
    
}

void ASTLiteralNode::scopeAnalyze(){
    //nothing
    
}

ASTLiteralNode * ASTLiteralNode::calc() {
	return this;
}

void ASTLiteralNode::printNode(int indent, ostream * output) {
	ostringstream oss;
	
	this->output = output;
	
	printIndented("literal", indent);
	printIndented("type: " + Scanner::namesRev[type], indent + 2);
	oss << "value: " << value;
	printIndented(oss.str(), indent + 2);
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}