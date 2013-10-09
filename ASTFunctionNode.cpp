/* 
 * File:   ASTFunctionNode.cpp
 * Author: claire
 * 
 * Created on October 6, 2013, 2:47 PM
 */

#include "ASTFunctionNode.h"

ASTFunctionNode::ASTFunctionNode() : ASTDeclarationNode(), param(NULL), compound(NULL) {
}

ASTFunctionNode::ASTFunctionNode(const ASTFunctionNode& orig):ASTDeclarationNode(orig),param(orig.param),
        compound(orig.compound)
{
}

ASTFunctionNode& ASTFunctionNode::operator= (const ASTFunctionNode &rhs)
{
	ASTDeclarationNode::operator=(rhs);
	
    // do the copy
        param = rhs.param;
        compound = rhs.compound;
 
    // return the existing object
    return *this;
}

ASTFunctionNode::~ASTFunctionNode() {
    delete param;
    delete compound;
}

void ASTFunctionNode::printNode(int indent, ostream * output) {
	this->output = output;
	
	printIndented("function", indent);
	printIndented("id: " + idName, indent + 2);
	printIndented("type: " + Scanner::namesRev[declarationType], indent + 2);
	if(param != NULL) {
		printIndented("params:", indent + 2);
		param->printNode(indent + 4, output);
	}
	printIndented("body:", indent + 2);
	compound->printNode(indent + 4, output);
	
	if(next != NULL) {
		next->printNode(indent, output);
	}
}